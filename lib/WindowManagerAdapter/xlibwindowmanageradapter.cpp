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
#include <QtCore/QtDebug>

// Should be included after all Qt includes
// or gives strange Status definition errors.
#include "xlibwindowmanageradapter_p.h"

int XLibWindowManagerAdapterPrivate::xErrorHandler(Display* display,
                                                   XErrorEvent* error)
{
    (void)display;
    (void)error;
    // qFatal("XLibWindowManagerAdapterPrivate::xErrorHandler called");
    qWarning("XLibWindowManagerAdapterPrivate::xErrorHandler called");
    return 1;
}

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

    root = XDefaultRootWindow(m_display);
    XSelectInput(m_display, root, PropertyChangeMask);
    XMapWindow(m_display, root);
    XFlush(m_display);

    m_mask.deviceid = XIAllMasterDevices;
    m_mask.mask_len = XIMaskLen(XI_LASTEVENT);
    m_mask.mask = (unsigned char*)calloc(m_mask.mask_len, sizeof(unsigned char));

    XISetMask(m_mask.mask, XI_TouchBegin);
    XISetMask(m_mask.mask, XI_TouchUpdate);
    XISetMask(m_mask.mask, XI_TouchEnd);
    XISetMask(m_mask.mask, XI_TouchOwnership);
    XISetMask(m_mask.mask, XI_HierarchyChanged);

    XIGrabTouchBegin(m_display, XIAllMasterDevices, root, 0,
                     &m_mask, 1, &m_mods);
}

XLibWindowManagerAdapterPrivate::~XLibWindowManagerAdapterPrivate()
{
    free(m_mask.mask);
    XFree(m_clients);
    XCloseDisplay(m_display);
}

void XLibWindowManagerAdapterPrivate::onNewEvent()
{
    XSetErrorHandler(xErrorHandler);
    XSync(m_display, False);
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
            if (q_ptr->m_listener) {
                q_ptr->m_listener->onTouchEvent(xcookie);
                // In case TouchManager changes error handler
                XSetErrorHandler(xErrorHandler);
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
    QString targetName;
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
        qDebug() << "Created: " << window << " " << targetName;
        q_ptr->m_listener->onWindowCreated(window, targetName, &grabTouches);
        if (grabTouches) {
            XIGrabTouchBegin(m_display, XIAllMasterDevices, window, 0,
                             &m_mask, 1, &m_mods);
        }
    }
}

void XLibWindowManagerAdapterPrivate::handleDestroyedWindow(Window window)
{
    if(q_ptr->m_listener) {
        qDebug() << "Destroyed: " << window;
        q_ptr->m_listener->onWindowDestroyed(window);
    }
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

void* XLibWindowManagerAdapter::display()
{
    return d_ptr->m_display;
}
