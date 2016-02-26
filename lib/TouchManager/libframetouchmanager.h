/* Copyright (C) 2015-2016 Gökhan Karabulut <gokhan.karabulut@tubitak.gov.tr>
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

#ifndef LIBFRAMETOUCHMANAGER_H
#define LIBFRAMETOUCHMANAGER_H

#include "touchmanager.h"
#include <QtCore/QObject>
#include <QtCore/QHash>
#include <X11/extensions/XInput2.h>
#include <oif/frame.h>

class QSocketNotifier;

class LibFrameTouchManager : public QObject,
                             public TouchManager
{
    Q_OBJECT
public:
    explicit LibFrameTouchManager(Display *display = nullptr,
                                  QObject *parent = nullptr);
    virtual ~LibFrameTouchManager();
    virtual void processTouchEvent(void *data);
    virtual void acceptTouch(unsigned long touchId, unsigned long targetId,
                             void* device);
    virtual void rejectTouch(unsigned long touchId, unsigned long targetId,
                             void* device);

    void setDisplay(Display* display)
    {m_display = display;}
    Display* display()
    {return m_display;}
private slots:
    void onFrameEvent();
private:
    struct DispatchInfo
    {
        DispatchInfo()
            :DispatchInfo(0.0f, 0.0f, 0){}
        DispatchInfo(float x, float y, int c)
            :rootStartX(x), rootStartY(y), rootCounter(c){}
        float rootStartX;
        float rootStartY;
        int rootCounter;
    };

    void getAxisInfo(UFAxis axis, UFAxisType *type,
                    const char **name, float *min,
                       float *max, float *res);
    void getDeviceLimits(UFDevice device,
                        float *minX, float *minY, float *maxX, float *maxY,
                         float *resx, float *resy, int *device_direct);
    void onDeviceAdded(UFEvent event);
    void onDeviceRemoved(UFEvent event);
    void onNewFrame(UFEvent event);
    void dispatchTouches(UFTouch touch,
                         UFDevice device, Window window);
    void accept_touch(unsigned long touchId, unsigned long targetId,
                             void* device);
    void reject_touch(unsigned long touchId, unsigned long targetId,
                             void* device);

    UFHandle m_frameHandle;
    Display* m_display;
    QSocketNotifier* m_socketNotifier;
    bool m_shouldCloseDisplay;
    QHash <uint32_t, DispatchInfo> m_dispatchHash;
    static const int TARGET_BOUND = 4;

    static int xErrorHandler(Display* display, XErrorEvent* error);
};

#endif /* LIBFRAMETOUCHMANAGER_H */
