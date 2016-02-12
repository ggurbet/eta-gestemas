/* Copyright (C) 2015 Gökhan Karabulut <gokhan.karabulut@tubitak.gov.tr>
 *
 * This file is part of eta-gestemas.
 *
 * eta-gestemas is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * eta-gestemas is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with eta-gestemas.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtCore/QSocketNotifier>
#include <QtCore/QtGlobal>
#include <QtCore/QtDebug>
#include "libframetouchmanager.h"
#include "gesturerecognizermanager.h"
#include <oif/frame_x11.h>
#include <cstdint>

int LibFrameTouchManager::xErrorHandler(Display* display,
                                                   XErrorEvent* error)
{
    (void)display;
    (void)error;
    // qFatal("LibFrameTouchManager::xErrorHandler called");
    qWarning("LibFrameTouchManager::xErrorHandler called");
    return 1;
}

LibFrameTouchManager::LibFrameTouchManager(Display* display,
                                           QObject *parent)
    :QObject(parent), m_display(nullptr),
     m_shouldCloseDisplay(false)
{
    UFStatus status;
    if (!display) {
        m_display = XOpenDisplay(nullptr);
        Q_CHECK_PTR(m_display);
        m_shouldCloseDisplay = true;
    } else {
        m_display = display;
        m_shouldCloseDisplay = false;
    }
    status = frame_x11_new(m_display, &m_frameHandle);
    Q_ASSERT(status == UFStatusSuccess);

    m_socketNotifier =
        new QSocketNotifier(frame_get_fd(m_frameHandle),
                            QSocketNotifier::Read, this);
    Q_CHECK_PTR(m_socketNotifier);
    m_socketNotifier->setEnabled(true);
    connect(m_socketNotifier, SIGNAL(activated(int)),
            this, SLOT(onFrameEvent()));
}

LibFrameTouchManager::~LibFrameTouchManager()
{
    if (m_shouldCloseDisplay) {
        XCloseDisplay(m_display);
    }
    frame_x11_delete(m_frameHandle);
}
void LibFrameTouchManager::processTouchEvent(void *data)
{
    XGenericEventCookie *xcookie = (XGenericEventCookie*)data;
    UFStatus status;
    status = frame_x11_process_event(m_frameHandle, xcookie);
    if (status != UFStatusSuccess) {
        qWarning("Failed to inject X event");
    }
}

void LibFrameTouchManager::acceptTouch(unsigned long touchId,
                                       unsigned long targetId,
                                       void* device)
{
    (void)targetId;
    accept_touch(touchId, XDefaultRootWindow(m_display), device);
}

void LibFrameTouchManager::rejectTouch(unsigned long touchId,
                                       unsigned long targetId,
                                       void* device)
{
    (void)targetId;
    reject_touch(touchId, XDefaultRootWindow(m_display), device);
}

void LibFrameTouchManager::accept_touch(unsigned long touchId,
                                        unsigned long targetId,
                                        void* device)
{
    UFTouchId ufTouchId = frame_x11_create_touch_id(touchId);
    UFWindowId ufWindowId = frame_x11_create_window_id(targetId);
    UFDevice ufDevice = static_cast<UFDevice>(device);
    if (frame_accept_touch(ufDevice, ufWindowId, ufTouchId) != UFStatusSuccess) {
        qWarning() << "Failed to accept touch " << touchId << " " << targetId;
    }
}

void LibFrameTouchManager::reject_touch(unsigned long touchId,
                                        unsigned long targetId,
                                        void* device)
{
    UFTouchId ufTouchId = frame_x11_create_touch_id(touchId);
    UFWindowId ufWindowId = frame_x11_create_window_id(targetId);
    UFDevice ufDevice = static_cast<UFDevice>(device);
    if (frame_reject_touch(ufDevice, ufWindowId, ufTouchId) != UFStatusSuccess) {
        qWarning() << "Failed to reject touch " << touchId << " " << targetId;
    }
}

void LibFrameTouchManager::onFrameEvent()
{
    UFEvent event;
    XSetErrorHandler(xErrorHandler);
    while (frame_get_event(m_frameHandle, &event) == UFStatusSuccess) {
        switch (frame_event_get_type(event)) {
        case UFEventTypeDeviceAdded:
            onDeviceAdded(event);
            break;
        case UFEventTypeDeviceRemoved:
            onDeviceRemoved(event);
            break;
        case UFEventTypeFrame:
            onNewFrame(event);
            break;
        }
        frame_event_unref(event);
    }
}

void LibFrameTouchManager::onDeviceAdded(UFEvent event)
{
    UFDevice device;
    char *string = NULL;
    UFStatus status;
    int num_axes = 0;
    int integer;
    int i;

    status = frame_event_get_property(event, UFEventPropertyDevice, &device);
    if (status != UFStatusSuccess) {
        qWarning() << "Failed to get device from event";
        return;
    }

    qDebug() << "Device Added:";
    qDebug() << "Time: " << frame_event_get_time(event) << " ms";

    status = frame_device_get_property(device, UFDevicePropertyName, &string);
    if (status != UFStatusSuccess)
        qWarning() << "Failed to get name from device";
    else
        qDebug() << "Name: " << string;

    status = frame_device_get_property(device, UFDevicePropertyDirect, &integer);
    if (status != UFStatusSuccess)
        qWarning() << "Failed to get direct roperty from device";
    else
        qDebug() << "Direct: " << (integer ? "yes" : "no");

    status = frame_device_get_property(device, UFDevicePropertyIndependent,
                                       &integer);
    if (status != UFStatusSuccess)
        qWarning() << "Failed to get independent property from device";
    else
        qDebug() << " Independent: " << (integer ? "yes" : "no");

    status = frame_device_get_property(device, UFDevicePropertySemiMT, &integer);
    if (status != UFStatusSuccess)
        qWarning() << "Failed to get semi-MT property from device";
    else
        qDebug() << "  Semi-MT: " << (integer ? "yes" : "no");

    status = frame_device_get_property(device, UFDevicePropertyMaxTouches,
                                       &integer);
    if (status != UFStatusSuccess)
        qWarning() << "Error: failed to get max touches from device";
    else
        qDebug() << " Maximum touches: " << integer;

    num_axes = frame_device_get_num_axes(device);
    qDebug() << " Number of axes: " << num_axes;

    UFAxis axis;
    UFAxisType type;
    const char *name;
    float min;
    float max;
    float res;
    for (i = 0; i < num_axes; ++i) {

        printf("  Axis %d:\n", i);
        qDebug() << "Axis " << i <<":";

        status = frame_device_get_axis_by_index(device, i, &axis);
        if (status != UFStatusSuccess) {
            qWarning() << "Failed to get axis " << i << " from device";
            continue;
        }

        getAxisInfo(axis, &type, &name, &min, &max, &res);
        qDebug() << "    Type: " << name;
        qDebug() << "    Minimum: " << min;
        qDebug() << "    Maximum: " << max;
        qDebug() << "    Resolution: " << res;
    }
    qDebug();
}

void LibFrameTouchManager::onDeviceRemoved(UFEvent event)
{
    UFDevice device;
    char *string = NULL;
    UFStatus status;

    status = frame_event_get_property(event, UFEventPropertyDevice, &device);
    if (status != UFStatusSuccess) {
        qWarning() << "Failed to get device from event";
        return;
    }

    qDebug() << "Device removed:";
    qDebug() << "Time: " << frame_event_get_time(event) << " ms";

    status = frame_device_get_property(device, UFDevicePropertyName, &string);
    if (status != UFStatusSuccess)
        qWarning() << "Failed to get name from device";
    else
        qDebug() << "Name: " << string;
}

void LibFrameTouchManager::onNewFrame(UFEvent event)
{
    UFFrame frame;
    UFDevice device;
    UFStatus status;
    Window window;
    int num_touches = 0;
    int i;

    status = frame_event_get_property(event, UFEventPropertyFrame, &frame);
    if (status != UFStatusSuccess) {
        qWarning() << "Failed to get frame from frame event";
        return;
    }

    window = frame_x11_get_window_id(frame_frame_get_window_id(frame));
    device = frame_frame_get_device(frame);
    num_touches = frame_frame_get_num_touches(frame);

    UFTouch touch;
    for (i = 0; i < num_touches; ++i) {
        status = frame_frame_get_touch_by_index(frame, i, &touch);
        if (status != UFStatusSuccess) {
            qWarning("Failed to get touch %d from frame", i);
            continue;
        }
        dispatchTouches(touch, device, window);
    }
}

void LibFrameTouchManager::dispatchTouches(UFTouch touch,
                                           UFDevice device, Window window)
{
    // This is messy!
    uint32_t touchId = 0UL;
    float x = 0.0f;
    float y = 0.0f;
    int device_direct = 0;
    float resolutionX = 0.0f;
    float resolutionY = 0.0f;
    float minX = 0.0f;
    float minY = 0.0f;
    float maxX = 0.0f;
    float maxY = 0.0f;
    uint64_t timestamp = 0ULL;
    Window root = XDefaultRootWindow(m_display);

    touchId = frame_x11_get_touch_id(frame_touch_get_id(touch));
    getDeviceLimits(device, &minX, &minY, &maxX, &maxY,
                    &resolutionX, &resolutionY, &device_direct);
    if (device_direct) {
        x = frame_touch_get_window_x(touch);
        y = frame_touch_get_window_y(touch);
    } else {
        x = frame_touch_get_device_x(touch);
        y = frame_touch_get_device_y(touch);
    }
    timestamp = frame_touch_get_time(touch);
    Q_ASSERT(m_grm != nullptr);

    switch (frame_touch_get_state(touch)) {
    case UFTouchStateBegin:
        // qDebug() << "Began " << touchId << window;
        if (window == root) {
            DispatchInfo di(x, y, 1);
            m_dispatchHash[touchId] = di;
        } else {
            DispatchInfo &di = m_dispatchHash[touchId];
            di.rootCounter = 0;
            x = di.rootStartX;
            y = di.rootStartY;
            reject_touch(touchId, window, device);
            m_grm->onTouchBegan(touchId, x, y, minX, minY,
                                maxX, maxY, window, device, timestamp);
        }
        break;
    case UFTouchStateUpdate:
        // qDebug() << "Update " << touchId << " " << window;
        if (window == root) {
            if (m_dispatchHash[touchId].rootCounter == 0) {
                m_grm->onTouchUpdated(touchId, x, y, timestamp);
            } else if (++m_dispatchHash[touchId].rootCounter == TARGET_BOUND) {
                reject_touch(touchId, window, device);
            }
        }
        break;
    case UFTouchStateEnd:
        // qDebug() << "Ended " << touchId << " " << window;
        if (window == root) {
            if (m_dispatchHash[touchId].rootCounter == 0) {
                m_grm->onTouchEnded(touchId, x, y, timestamp);
            } else if (m_dispatchHash[touchId].rootCounter < TARGET_BOUND) {
                reject_touch(touchId, window, device);
            }
            m_dispatchHash.remove(touchId);
        }
        break;
    }
}

void LibFrameTouchManager::getAxisInfo(UFAxis axis, UFAxisType *type,
                                       const char **name, float *min,
                                       float *max, float *res) {
    *type = frame_axis_get_type(axis);

    switch (*type) {
    case UFAxisTypeX:
        *name = "X";
        break;

    case UFAxisTypeY:
        *name = "Y";
        break;

    case UFAxisTypeTouchMajor:
        *name = "Touch major";
        break;

    case UFAxisTypeTouchMinor:
        *name = "Touch minor";
        break;

    case UFAxisTypeWidthMajor:
        *name = "Width major";
        break;

    case UFAxisTypeWidthMinor:
        *name = "Width minor";
        break;

    case UFAxisTypeOrientation:
        *name = "Orientation";
        break;

    case UFAxisTypeTool:
        *name = "Tool";
        break;

    case UFAxisTypeBlobId:
        *name = "Blob ID";
        break;

    case UFAxisTypeTrackingId:
        *name = "Tracking ID";
        break;

    case UFAxisTypePressure:
        *name = "Pressure";
        break;

    case UFAxisTypeDistance:
        *name = "Distance";
        break;

    default:
        *name = "Unknown";
        break;
    }

    *min = frame_axis_get_minimum(axis);
    *max = frame_axis_get_maximum(axis);
    *res = frame_axis_get_resolution(axis);
}

void LibFrameTouchManager::getDeviceLimits(UFDevice device,
                                           float *minX, float *minY, float *maxX, float *maxY,
                                               float *resx, float *resy, int *device_direct)
{
    UFStatus status = frame_device_get_property(device, UFDevicePropertyDirect,
                                                device_direct);
    if (status != UFStatusSuccess) {
        qFatal("Failed to get property direct");
        return;
    }

    if (*device_direct) {
        *resx = frame_device_get_window_resolution_x(device);
        *resy = frame_device_get_window_resolution_y(device);
        *minX = 0.0f;
        *minY = 0.0f;
        *maxX = DisplayWidth(m_display, DefaultScreen(m_display));
        *maxY = DisplayHeight(m_display, DefaultScreen(m_display));

        /* If resolution is not available, assume 96 dpi and convert to meters */
        if (*resx <= 0)
            *resx = 3780;
        if (*resy <= 0)
            *resy = 3780;
    } else {
        *resx = 46000; /* Default to resolution of Apple Magic Trackpad */
        *resy = 45000;

        UFAxis axis;
        status = frame_device_get_axis_by_type(device, UFAxisTypeX, &axis);
        if (status != UFStatusSuccess)
            qFatal("Failed to get X axis from device");
        else if (frame_axis_get_resolution(axis) > 0) {
            *resx = frame_axis_get_resolution(axis);
            *minX = frame_axis_get_minimum(axis);
            *maxX = frame_axis_get_maximum(axis);
        }

        status = frame_device_get_axis_by_type(device, UFAxisTypeY, &axis);
        if (status != UFStatusSuccess)
            qFatal("Failed to get Y axis from device");
        else if (frame_axis_get_resolution(axis) > 0) {
            *resy = frame_axis_get_resolution(axis);
            *minY = frame_axis_get_minimum(axis);
            *maxY = frame_axis_get_maximum(axis);
        }
    }
}
