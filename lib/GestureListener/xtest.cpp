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
 * Mostly adopted from ginn.
 * original author : Mohamed-Ikbel Boulabiar <boulabiar@gmail.com>
 */

#include "gesturerecognizer.h"
#include <QtCore/QStringList>
#include <QtCore/QtDebug>
#include "xtest.h"

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

void XTest::injectButtonPress(int btn, const QStringList& modifiers)
{
    char *modifier = nullptr;
    for (int i = 0; i < modifiers.size(); ++i) {
        modifier = modifiers.at(i).toLatin1().data();
        XTestFakeKeyEvent(disp,
                          XKeysymToKeycode(disp, XStringToKeysym(modifier)),
                          True, CurrentTime);
    }
    XTestFakeButtonEvent(disp, btn, True, CurrentTime);
    XFlush(disp);
}

void XTest::injectButtonRelease(int btn, const QStringList& modifiers)
{
    char *modifier = nullptr;
    XTestFakeButtonEvent(disp, btn, False, CurrentTime);
    for (int i = 0; i < modifiers.size(); ++i) {
        modifier = modifiers.at(i).toLatin1().data();
        XTestFakeKeyEvent(disp,
                          XKeysymToKeycode(disp, XStringToKeysym(modifier)),
                          False, CurrentTime);
    }
    XFlush(disp);
}

void XTest::injectButtonPress(int btn)
{
    XTestFakeButtonEvent(disp, btn, True, CurrentTime);
    XFlush(disp);
}

void XTest::injectButtonRelease(int btn)
{
    XTestFakeButtonEvent(disp, btn, False, CurrentTime);
    XFlush(disp);
}


void XTest::injectButtonPressRelease(int btn, const QStringList& modifiers)
{
    char *modifier = nullptr;
    for (int i = 0; i < modifiers.size(); ++i) {
        modifier = modifiers.at(i).toLatin1().data();
        XTestFakeKeyEvent(disp,
                          XKeysymToKeycode(disp, XStringToKeysym(modifier)),
                          True, CurrentTime);
    }
    XTestFakeButtonEvent(disp, btn, True, CurrentTime);
    XTestFakeButtonEvent(disp, btn, False, CurrentTime);
    for (int i = 0; i < modifiers.size(); ++i) {
        modifier = modifiers.at(i).toLatin1().data();
        XTestFakeKeyEvent(disp,
                          XKeysymToKeycode(disp, XStringToKeysym(modifier)),
                          False, CurrentTime);
    }
    XFlush(disp);
}

void XTest::injectButtonPressRelease(int btn)
{
    XTestFakeButtonEvent(disp, btn, True, CurrentTime);
    XTestFakeButtonEvent(disp, btn, False, CurrentTime);
    XFlush(disp);
}

void XTest::injectKeyPress(const QString& key, const QStringList& modifiers)
{
    char *modifier = nullptr;
    for (int i = 0; i < modifiers.size(); ++i) {
        modifier = modifiers.at(i).toLatin1().data();
        XTestFakeKeyEvent(disp,
                          XKeysymToKeycode(disp, XStringToKeysym(modifier)),
                          True, CurrentTime);
    }
    char *kstr = key.toLatin1().data();
    XTestFakeKeyEvent(disp,
                      XKeysymToKeycode(disp, XStringToKeysym(kstr)),
                      True, CurrentTime);
    XFlush(disp);
}

void XTest::injectKeyRelease(const QString& key, const QStringList& modifiers)
{
    char *modifier = nullptr;
    char *kstr = key.toLatin1().data();
    XTestFakeKeyEvent(disp,
                      XKeysymToKeycode(disp, XStringToKeysym(kstr)),
                      False, CurrentTime);
    for (int i = 0; i < modifiers.size(); ++i) {
        modifier = modifiers.at(i).toLatin1().data();
        XTestFakeKeyEvent(disp,
                          XKeysymToKeycode(disp, XStringToKeysym(modifier)),
                          False, CurrentTime);
    }
    XFlush(disp);
}

void XTest::injectKeyPress(const QString& key)
{
    char *kstr = key.toLatin1().data();
    XTestFakeKeyEvent(disp,
                      XKeysymToKeycode(disp, XStringToKeysym(kstr)),
                      True, CurrentTime);
    XFlush(disp);
}

void XTest::injectKeyRelease(const QString& key)
{
    char *kstr = key.toLatin1().data();
    XTestFakeKeyEvent(disp,
                      XKeysymToKeycode(disp, XStringToKeysym(kstr)),
                      False, CurrentTime);
    XFlush(disp);
}

void XTest::injectKeyPressRelease(const QString& key, const QStringList& modifiers)
{
    char *modifier = nullptr;
    for (int i = 0; i < modifiers.size(); ++i) {
        modifier = modifiers.at(i).toLatin1().data();
        XTestFakeKeyEvent(disp,
                          XKeysymToKeycode(disp, XStringToKeysym(modifier)),
                          True, CurrentTime);
    }
    char *kstr = key.toLatin1().data();
    XTestFakeKeyEvent(disp,
                      XKeysymToKeycode(disp, XStringToKeysym(kstr)),
                      True, CurrentTime);
    XTestFakeKeyEvent(disp,
                      XKeysymToKeycode(disp, XStringToKeysym(kstr)),
                      False, CurrentTime);
    for (int i = 0; i < modifiers.size(); ++i) {
        modifier = modifiers.at(i).toLatin1().data();
        XTestFakeKeyEvent(disp,
                          XKeysymToKeycode(disp, XStringToKeysym(modifier)),
                          False, CurrentTime);
    }
    XFlush(disp);
}

void XTest::injectKeyPressRelease(const QString& key)
{
    char *kstr = key.toLatin1().data();
    XTestFakeKeyEvent(disp,
                      XKeysymToKeycode(disp, XStringToKeysym(kstr)),
                      True, CurrentTime);
    XTestFakeKeyEvent(disp,
                      XKeysymToKeycode(disp, XStringToKeysym(kstr)),
                      False, CurrentTime);
    XFlush(disp);
}
