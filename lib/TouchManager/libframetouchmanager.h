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

#ifndef LIBFRAMETOUCHMANAGER_H
#define LIBFRAMETOUCHMANAGER_H

#include "touchmanager.h"
#include <QtCore/QObject>
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
    void getAxisInfo(UFAxis axis, UFAxisType *type,
                    const char **name, float *min,
                       float *max, float *res);
    void getDeviceResolution(UFDevice device, float *resx, float *resy);
    void onDeviceAdded(UFEvent event);
    void onDeviceRemoved(UFEvent event);
    void onNewFrame(UFEvent event);
    void dispatchTouches(UFTouch touch,
                         UFDevice device, Window window);

    UFHandle m_frameHandle;
    Display* m_display;
    QSocketNotifier* m_socketNotifier;
    bool m_shouldCloseDisplay;
};

#endif /* LIBFRAMETOUCHMANAGER_H */
