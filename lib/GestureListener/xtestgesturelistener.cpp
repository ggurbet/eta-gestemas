/* Copyright (C) 2015 Gökhan Karabulut <gokhan.karabulut@tubitak.gov.tr>
 * Copyright (C) 2010 Canonical, Ltd.
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

/*
 * Mostly copied from ginn.
 * original author : Mohamed-Ikbel Boulabiar <boulabiar@gmail.com>
 */

#include "gesturerecognizer.h"
#include "xtestgesturelistener.h"
#include <cstring>

XTestGestureListener::XTestGestureListener(Display* display,
                     const GestureRecognizer *recognizer)
    :GestureListener(recognizer), m_shouldCloseDisplay(false)
{
    setDisplay(display);
}

XTestGestureListener::~XTestGestureListener()
{
    if (m_shouldCloseDisplay) {
        XCloseDisplay(m_display);
    }
}

void XTestGestureListener::setDisplay(Display* display)
{
    if (!display) {
        m_shouldCloseDisplay = true;
        m_display = XOpenDisplay(NULL);
    } else {
        m_shouldCloseDisplay = false;
        m_display = display;
    }
}

Display* XTestGestureListener::display() const
{
    return m_display;
}

void XTestGestureListener::movePointer()
{
    int targetX = static_cast<int>(m_recognizer->centralX());
    int targetY = static_cast<int>(m_recognizer->centralY());
    Window rootWindow;
    Window returnedWindow;
    Window targetWindow = m_recognizer->targetId();
    rootWindow = XDefaultRootWindow(m_display);
    int rootX = 0;
    int rootY = 0;
    bool coordinatesTranslated =
        XTranslateCoordinates(m_display,
                              targetWindow, rootWindow, targetX, targetY,
                              &rootX, &rootY, &returnedWindow);

    Q_ASSERT(coordinatesTranslated);
    XTestFakeMotionEvent(m_display, 0, rootX, rootY, CurrentTime);
}

void XTestGestureListener::injectKey(KeySym ks, const char *modifiers[])
{
    int i;

    for (i = 0; i < 4 && 0 != strcmp(modifiers[i], ""); i++)
        XTestFakeKeyEvent(m_display,
                          XKeysymToKeycode(m_display,
                                           XStringToKeysym(modifiers
                                                           [i])), True,
                          CurrentTime);
    XTestFakeKeyEvent(m_display, XKeysymToKeycode(m_display, ks), True, CurrentTime);
    XTestFakeKeyEvent(m_display, XKeysymToKeycode(m_display, ks), False,
                      CurrentTime);
    for (i = 0; i < 4 && 0 != strcmp(modifiers[i], ""); i++)
        XTestFakeKeyEvent(m_display,
                          XKeysymToKeycode(m_display,
                                           XStringToKeysym(modifiers
                                                           [i])), False,
                          CurrentTime);

    XFlush(m_display);
}

void XTestGestureListener::injectButton(int btn, const char *modifiers[])
{
    int i;

    for (i = 0; i < 4 && 0 != strcmp(modifiers[i], ""); i++)
        XTestFakeKeyEvent(m_display,
                          XKeysymToKeycode(m_display,
                                           XStringToKeysym(modifiers
                                                           [i])), True,
                          CurrentTime);
    XTestFakeButtonEvent(m_display, btn, True, CurrentTime);
    XTestFakeButtonEvent(m_display, btn, False, CurrentTime);
    for (i = 0; i < 4 && 0 != strcmp(modifiers[i], ""); i++)
        XTestFakeKeyEvent(m_display,
                          XKeysymToKeycode(m_display,
                                           XStringToKeysym(modifiers
                                                           [i])), False,
                                CurrentTime);

    XFlush(m_display);
}

void XTestGestureListener::injectMixed(KeySym ks, int btn, const char *modifiers[])
{
    int i;

    XTestFakeButtonEvent(m_display, btn, True, CurrentTime);
    for (i = 0; i < 4 && 0 != strcmp(modifiers[i], ""); i++)
        XTestFakeKeyEvent(m_display,
                          XKeysymToKeycode(m_display,
                                           XStringToKeysym(modifiers
                                                           [i])), True,
                          CurrentTime);
    XTestFakeKeyEvent(m_display, XKeysymToKeycode(m_display, ks), True, CurrentTime);
    XTestFakeKeyEvent(m_display, XKeysymToKeycode(m_display, ks), False,
                      CurrentTime);
    for (i = 0; i < 4 && 0 != strcmp(modifiers[i], ""); i++)
        XTestFakeKeyEvent(m_display,
                          XKeysymToKeycode(m_display,
                                           XStringToKeysym(modifiers
                                                           [i])), False,
                          CurrentTime);
    XTestFakeButtonEvent(m_display, btn, False, CurrentTime);
    XFlush(m_display);
}

void XTestGestureListener::injectLeftButtonPress()
{
    XTestFakeButtonEvent(m_display, 1, True, CurrentTime);
    XFlush(m_display);
}

void XTestGestureListener::injectLeftButtonRelease()
{
    XTestFakeButtonEvent(m_display, 1, False, CurrentTime);
    XFlush(m_display);
}
