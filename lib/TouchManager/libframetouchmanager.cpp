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
#include <oif/frame_x11.h>

LibFrameTouchManager::LibFrameTouchManager(Display* display)
    :m_display(nullptr)
{
    UFStatus status;
    Display* disp = nullptr;
    if (!display) {
        disp = XOpenDisplay(nullptr);
        Q_CHECK_PTR(disp);
        m_display = disp;
    } else {
        disp = display;
    }
    status = frame_x11_new(disp, &m_frameHandle);
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
    if (!m_display) {
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
    UFTouchId ufTouchId = frame_x11_create_touch_id(touchId);
    UFWindowId ufWindowId = frame_x11_create_window_id(targetId);
    UFDevice ufDevice = static_cast<UFDevice>(device);
    frame_accept_touch(ufDevice, ufWindowId, ufTouchId);
}
void LibFrameTouchManager::rejectTouch(unsigned long touchId,
                                       unsigned long targetId,
                                       void* device)
{
    UFTouchId ufTouchId = frame_x11_create_touch_id(touchId);
    UFWindowId ufWindowId = frame_x11_create_window_id(targetId);
    UFDevice ufDevice = static_cast<UFDevice>(device);
    frame_reject_touch(ufDevice, ufWindowId, ufTouchId);
}

void LibFrameTouchManager::onFrameEvent()
{
    UFEvent event;

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

static void get_axis_info(UFAxis axis, UFAxisType *type,
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

        get_axis_info(axis, &type, &name, &min, &max, &res);
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
    char *string = NULL;
    int num_touches = 0;
    int i;

    qDebug() << "Frame received:";
    qDebug() << "Time: " << frame_event_get_time(event) << " ms";

    status = frame_event_get_property(event, UFEventPropertyFrame, &frame);
    if (status != UFStatusSuccess) {
        qWarning() << "Failed to get frame from frame event";
        return;
    }

    window = frame_x11_get_window_id(frame_frame_get_window_id(frame));
    qDebug("Window: 0x%lx", window);

    device = frame_frame_get_device(frame);

    status = frame_device_get_property(device, UFDevicePropertyName, &string);
    if (status != UFStatusSuccess)
        qWarning() << "Failed to get device name from device";
    else
        qDebug() << "  Device: " << string;

    num_touches = frame_frame_get_num_touches(frame);
    qDebug() << "  Number of touches: " << num_touches;

    UFTouch touch;
    for (i = 0; i < num_touches; ++i) {
        qDebug("  Touch %d:", i);

        status = frame_frame_get_touch_by_index(frame, i, &touch);
        if (status != UFStatusSuccess) {
            qWarning("Failed to get touch %d from frame", i);
            continue;
        }

        dispatchTouches(touch, frame, device, window);
    }
    qDebug();
}

void LibFrameTouchManager::dispatchTouches(UFTouch touch, UFFrame frame,
                                           UFDevice device, Window window)
{
  UFStatus status;
  UFTouchState prev_state;
  int num_axes = 0;
  int prev_valid = 0;
  unsigned long long time;
  float floating;
  int owned;
  int pending_end;
  int j;

  qDebug("    ID: %u", frame_x11_get_touch_id(frame_touch_get_id(touch)));

  switch (frame_touch_get_state(touch)) {
    case UFTouchStateBegin:
        qDebug("    State: Begin");
      break;

    case UFTouchStateUpdate:
      qDebug("    State: Update");
      break;

    case UFTouchStateEnd:
      qDebug("    State: End");
      break;
  }
  qDebug("    Time: %ju", frame_touch_get_time(touch));
  qDebug("    Start Time: %ju", frame_touch_get_start_time(touch));
  qDebug("    Window X: %f", frame_touch_get_window_x(touch));
  qDebug("    Window Y: %f", frame_touch_get_window_y(touch));

  status = frame_touch_get_property(touch, UFTouchPropertyOwned, &owned);
  if (status != UFStatusSuccess)
    qWarning("Failed to get touch ownership property");
  else
    qDebug("    Owned: %s", owned ? "Yes" : "No");

  status = frame_touch_get_property(touch, UFTouchPropertyPendingEnd,
                                    &pending_end);
  if (status != UFStatusSuccess)
    qWarning("Failed to get touch pending end property");
  else
    qDebug("    Pending End: %s", pending_end ? "Yes" : "No");

  num_axes = frame_device_get_num_axes(device);

  UFAxis axis;
  UFAxisType type;
  const char *name;
  float min;
  float max;
  float res;
  float value;
  for (j = 0; j < num_axes; ++j) {
    status = frame_device_get_axis_by_index(device, j, &axis);
    if (status != UFStatusSuccess) {
      qWarning("Failed to get axis %d", j);
      continue;
    }

    get_axis_info(axis, &type, &name, &min, &max, &res);

    status = frame_touch_get_value(touch, type, &value);
    if (status != UFStatusSuccess) {
      qWarning("Failed to get value for axis %d", j);
      continue;
    }
    qDebug("Axis Value: %f (Axis: %s, Min: %f, Max: %f, Res: %f)", value, name,
           min, max, res);
  }
}
