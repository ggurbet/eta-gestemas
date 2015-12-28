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
 * along with ete-gestemas.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "xlibwindowmanageradapter.h"

#include <cstdlib>

#include <QtCore/QtGlobal>
#include <QtCore/QSocketNotifier>

// Should be included after all Qt includes
// or gives strange Status definition errors.
#include "xlibwindowmanageradapter_p.h"

XLibWindowManagerAdapterPrivate::XLibWindowManagerAdapterPrivate(
                                   XLibWindowManagerAdapter *q)
    :q_ptr(q)
{
    Window root;
    Status status;
    int xi_major = 2;
    int xi_minor = 2;
    m_mods.modifiers = XIAnyModifier;
    m_mods.status = 0;

    m_display = XOpenDisplay(NULL);
    Q_ASSERT_X(m_display, "XLibWindowManagerAdapter",
               "Failed to open connection to X server");

    status = XIQueryVersion(m_display, &xi_major, &xi_minor);
    Q_ASSERT_X(status == Success, "XLibWindowManagerAdapter",
               "Failed to query XInput version");

    if (xi_major < 2 || xi_minor < 2) {
        qFatal("XInput version of the server is too old (%d.%d)" ,
                  xi_major, xi_minor);
    }

    root = DefaultRootWindow(m_display);

    XMapWindow(m_display, root);
    XFlush(m_display);
    XSelectInput(m_display, root, PropertyChangeMask);

    m_mask.deviceid = XIAllMasterDevices;
    m_mask.mask_len = XIMaskLen(XI_LASTEVENT);
    m_mask.mask = (unsigned char*)calloc(m_mask.mask_len, sizeof(unsigned char));

    XISetMask(m_mask.mask, XI_TouchBegin);
    XISetMask(m_mask.mask, XI_TouchUpdate);
    XISetMask(m_mask.mask, XI_TouchEnd);
    XISetMask(m_mask.mask, XI_TouchOwnership);
    XISetMask(m_mask.mask, XI_HierarchyChanged);
}

XLibWindowManagerAdapterPrivate::~XLibWindowManagerAdapterPrivate()
{
    free(m_mask.mask);
    XFree(m_clients);
    XCloseDisplay(m_display);
}

void XLibWindowManagerAdapterPrivate::onNewEvent()
{
    XEvent event;
    while (XPending(m_display)) {
        XNextEvent(m_display, &event);
        if (event.type == PropertyNotify
            && event.xproperty.atom == XInternAtom(m_display,
                                                   "_NET_CLIENT_LIST", False)) {
            //Window Event
            Window *clients;
            unsigned long len = 0;
            if (getClients(&clients, &len)) {
                if (len > m_numberOfClients) {
                    // A  window created
                    handleCreatedWindow(clients[len - 1]);
                } else if (len < m_numberOfClients) {
                    // A window destroyed
                    unsigned long i, j;
                    for (i = 0; i < m_numberOfClients; ++i) {
                        for (j = 0; j < len; ++j) {
                            if (m_clients[i] == clients[j]) {
                                break;
                            }
                        }
                        if (j == len && i < m_numberOfClients) {
                            handleDestroyedWindow(m_clients[i]);
                            break;
                        }
                    }
                } else {
                    // This is strange, but happens.
                    qWarning("A window neither created nor destroyed");
                }
                // Set new clients and the number of clients
                XFree(m_clients);
                m_clients = clients;
                m_numberOfClients = len;
            }
        }
        if (event.type == GenericEvent) {
            //Touch Event
            XGenericEventCookie *xcookie = &event.xcookie;
            if (!XGetEventData(m_display, xcookie)) {
                qWarning("Failed to get X generic event data\n");
                continue;
            }
            //TODO: Fordward xcookie data to TouchManager here.
            if (q_ptr->m_listener) {
                q_ptr->m_listener->onTouchEvent(xcookie);
            }
            XFreeEventData(m_display, xcookie);
        }
    }
}

bool XLibWindowManagerAdapterPrivate::getClients(Window **clients,
                                                 unsigned long* len)
{
    int form;
    unsigned long remain;
    unsigned char *list;
    Status status;
    Atom type;
    Atom prop = XInternAtom(m_display, "_NET_CLIENT_LIST", False);

    status = XGetWindowProperty(m_display, XDefaultRootWindow(m_display),
                                prop, 0, 1024, False, XA_WINDOW, &type,
                                &form, len, &remain, &list);

    if (status == Success) {
        *clients = (Window *) list;
        return true;
    }
    return false;
}

void XLibWindowManagerAdapterPrivate::handleCreatedWindow(Window window)
{
    QString targetName = "root";
    if (window != XDefaultRootWindow(m_display)) {
        XClassHint *classHint = XAllocClassHint();
        XGetClassHint(m_display, window, classHint);
        targetName = QString(classHint->res_class);
        XFree(classHint->res_name);
        XFree(classHint->res_class);
        XFree(classHint);
    }
    if (q_ptr->m_listener) {
        bool grabTouches = true;
        q_ptr->m_listener->onWindowCreated(window, targetName, &grabTouches);
        if (grabTouches) {
            XIGrabTouchBegin(m_display, XIAllMasterDevices, window, 0,
                             &m_mask, 1, &m_mods);
        }
    }
}

void XLibWindowManagerAdapterPrivate::handleDestroyedWindow(Window window)
{
    //     A window neither created nor destroyed
    //     A window neither created nor destroyed
    //     Target Destroyed: 0x3602791
    //     X Error of failed request:  BadWindow (invalid Window parameter)
    //     Major opcode of failed request:  131 (XInputExtension)
    //     Minor opcode of failed request:  55 ()
    //     Resource id in failed request:  0x3602791
    //     Serial number of failed request:  50
    //     Current serial number in output stream:  50


    // Reproduced on krunner system activity close
    /* Possible solutions are
      1-)Search destroyed window id in GestureRecognizerManager
      if not found, do not ungrab.
      2-) Consider implementing following and setting and defaulting error handler
      before a call that might fail.
      int (*XSetErrorHandler(int (*handler)(Display *, XErrorEvent *)))();
      int (*XSetIOErrorHandler(int (*handler)(Display *)))();
      3-) Do something in the case "A window neither created nor distroyed"
      4-) Do not ungrab at all.
     */

    if(q_ptr->m_listener) {
        q_ptr->m_listener->onWindowDestroyed(window);
    }
    // XIUngrabTouchBegin(m_display, XIAllMasterDevices, window, 1, &m_mods);

    //TODO: Forward window to GestureRecognizerManager here.
}

void XLibWindowManagerAdapterPrivate::dispatchOpenWindowEvents()
{
    Window root = XDefaultRootWindow(m_display);
    handleCreatedWindow(root);

    getClients(&m_clients, &m_numberOfClients);
    for (unsigned long i = 0; i < m_numberOfClients; ++i) {
        handleCreatedWindow(m_clients[i]);
    }
}

/**
 * @class XLibWindowManagerAdapter
 * @brief   Window manager adapter for Xlib.
 *
 * This class uses Xlib as a backend to capture window creation,
 * window destruction and touch events.
 */

/**
 * @brief   Constructor inits private members.
 *
 * @param[in] parent    deletes this instance automatically
 * @return              An instance of this class.
 */
XLibWindowManagerAdapter::XLibWindowManagerAdapter(QObject *parent)
    :WindowManagerAdapter(parent),
     d_ptr(new XLibWindowManagerAdapterPrivate(this))
{
}

/**
 * @brief   Destructor deletes private members.
 */
XLibWindowManagerAdapter::~XLibWindowManagerAdapter()
{
    delete d_ptr;
    d_ptr = nullptr;
}

void XLibWindowManagerAdapter::onNewEvent()
{
    d_ptr->onNewEvent();
}

void XLibWindowManagerAdapter::dispatchEvents()
{
    d_ptr->dispatchOpenWindowEvents();

    d_ptr->m_socketNotifier =
        new QSocketNotifier(ConnectionNumber(d_ptr->m_display),
                                           QSocketNotifier::Read, this);
    Q_CHECK_PTR(d_ptr->m_socketNotifier);
    d_ptr->m_socketNotifier->setEnabled(true);
    connect(d_ptr->m_socketNotifier, SIGNAL(activated(int)),
            this, SLOT(onNewEvent()));
}
