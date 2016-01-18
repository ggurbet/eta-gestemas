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

#ifndef XTESTGESTURELISTENER_H
#define XTESTGESTURELISTENER_H

#include "gesturelistener.h"
#include <X11/X.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>

class XTestGestureListener : public GestureListener
{
public:
    XTestGestureListener(Display* display = nullptr);
    virtual ~XTestGestureListener();

    void setDisplay(Display* display = nullptr);
    Display* display() const;
protected:
    void movePointer();
    void injectKey(KeySym ks, const char *modifiers[]);
    void injectButton(int btn, const char *modifiers[]);
    void injectMixed(KeySym ks, int btn, const char *modifiers[]);
    void injectLeftButtonPress();
    void injectLeftButtonRelease();
    void injectKeyPress(KeySym ks);
    void injectKeyRelease(KeySym ks);

    Display* m_display;
    bool m_shouldCloseDisplay;
};

#endif /* XTESTGESTURELISTENER_H */
