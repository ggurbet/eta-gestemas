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

#ifndef XTEST_H
#define XTEST_H

#include <X11/X.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>

class XTest
{
public:
    static void open(Display* display = nullptr);
    static void close();
    static Display* display();
    static void movePointer(float x, float y);
    static void injectKey(KeySym ks, const char *modifiers[]);
    static void injectButton(int btn, const char *modifiers[]);
    static void injectMixed(KeySym ks, int btn, const char *modifiers[]);
    static void injectLeftButtonPress();
    static void injectLeftButtonRelease();
    static void injectKeyPress(KeySym ks);
    static void injectKeyRelease(KeySym ks);

private:
    static Display* disp;
    static bool shouldCloseDisplay;
};

#endif /* XTEST_H */
