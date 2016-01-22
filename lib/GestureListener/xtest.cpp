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
#include "xtest.h"
#include <cstring>

Display* XTest::disp = nullptr;
bool XTest::shouldCloseDisplay = false;

void XTest::open(Display* display)
{
    shouldCloseDisplay = false;
    if (!display) {
        shouldCloseDisplay = true;
        disp = XOpenDisplay(NULL);
    } else {
        shouldCloseDisplay = false;
        disp = display;
    }
}

void XTest::close()
{
    if (shouldCloseDisplay) {
        XCloseDisplay(disp);
    }
}

Display* XTest::display()
{
    return disp;
}

void XTest::movePointer(float x, float y)
{
    int targetX = static_cast<int>(x + 0.5f);
    int targetY = static_cast<int>(y + 0.5f);
    XTestFakeMotionEvent(disp, 0, targetX, targetY, CurrentTime);
    XFlush(disp);
}

void XTest::injectKey(KeySym ks, const char *modifiers[])
{
    int i;

    for (i = 0; i < 4 && 0 != strcmp(modifiers[i], ""); i++)
        XTestFakeKeyEvent(disp,
                          XKeysymToKeycode(disp,
                                           XStringToKeysym(modifiers
                                                           [i])), True,
                          CurrentTime);
    XTestFakeKeyEvent(disp, XKeysymToKeycode(disp, ks), True, CurrentTime);
    XTestFakeKeyEvent(disp, XKeysymToKeycode(disp, ks), False,
                      CurrentTime);
    for (i = 0; i < 4 && 0 != strcmp(modifiers[i], ""); i++)
        XTestFakeKeyEvent(disp,
                          XKeysymToKeycode(disp,
                                           XStringToKeysym(modifiers
                                                           [i])), False,
                          CurrentTime);

    XFlush(disp);
}

void XTest::injectButton(int btn, const char *modifiers[])
{
    int i;

    for (i = 0; i < 4 && 0 != strcmp(modifiers[i], ""); i++)
        XTestFakeKeyEvent(disp,
                          XKeysymToKeycode(disp,
                                           XStringToKeysym(modifiers
                                                           [i])), True,
                          CurrentTime);
    XTestFakeButtonEvent(disp, btn, True, CurrentTime);
    XTestFakeButtonEvent(disp, btn, False, CurrentTime);
    for (i = 0; i < 4 && 0 != strcmp(modifiers[i], ""); i++)
        XTestFakeKeyEvent(disp,
                          XKeysymToKeycode(disp,
                                           XStringToKeysym(modifiers
                                                           [i])), False,
                                CurrentTime);

    XFlush(disp);
}

void XTest::injectMixed(KeySym ks, int btn, const char *modifiers[])
{
    int i;

    XTestFakeButtonEvent(disp, btn, True, CurrentTime);
    for (i = 0; i < 4 && 0 != strcmp(modifiers[i], ""); i++)
        XTestFakeKeyEvent(disp,
                          XKeysymToKeycode(disp,
                                           XStringToKeysym(modifiers
                                                           [i])), True,
                          CurrentTime);
    XTestFakeKeyEvent(disp, XKeysymToKeycode(disp, ks), True, CurrentTime);
    XTestFakeKeyEvent(disp, XKeysymToKeycode(disp, ks), False,
                      CurrentTime);
    for (i = 0; i < 4 && 0 != strcmp(modifiers[i], ""); i++)
        XTestFakeKeyEvent(disp,
                          XKeysymToKeycode(disp,
                                           XStringToKeysym(modifiers
                                                           [i])), False,
                          CurrentTime);
    XTestFakeButtonEvent(disp, btn, False, CurrentTime);
    XFlush(disp);
}

void XTest::injectLeftButtonPress()
{
    XTestFakeButtonEvent(disp, 1, True, CurrentTime);
    XFlush(disp);
}

void XTest::injectLeftButtonRelease()
{
    XTestFakeButtonEvent(disp, 1, False, CurrentTime);
    XFlush(disp);
}

void XTest::injectKeyPress(KeySym ks)
{
    XTestFakeKeyEvent(disp, XKeysymToKeycode(disp, ks), True,
                      CurrentTime);
    XFlush(disp);
}

void XTest::injectKeyRelease(KeySym ks)
{
    XTestFakeKeyEvent(disp, XKeysymToKeycode(disp, ks), False,
                      CurrentTime);
    XFlush(disp);
}
