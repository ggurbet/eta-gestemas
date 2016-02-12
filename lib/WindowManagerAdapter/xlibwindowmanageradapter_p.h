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

#ifndef XLIBWINDOWMANAGERADAPTER_P_H
#define XLIBWINDOWMANAGERADAPTER_P_H

#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

class QSocketNotifier;
class XLibWindowManagerAdapter;

class XLibWindowManagerAdapterPrivate {
public:
    XLibWindowManagerAdapterPrivate(XLibWindowManagerAdapter *q);
    ~XLibWindowManagerAdapterPrivate();

    bool getClients(Window **clients, unsigned long *len);
    void handleCreatedWindow(Window window);
    void handleDestroyedWindow(Window window);
    void onNewEvent();
    void dispatchOpenWindowEvents();

    QSocketNotifier *m_socketNotifier;
    Display *m_display;
    Window *m_clients;
    unsigned long m_numberOfClients;
    XIEventMask m_mask;
    XIGrabModifiers m_mods;

    XLibWindowManagerAdapter *q_ptr;
    static int xErrorHandler(Display* display, XErrorEvent* error);
};

#endif
