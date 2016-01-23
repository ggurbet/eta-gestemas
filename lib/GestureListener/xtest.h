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

class QStringList;
class QString;

class XTest
{
public:
    static void open(Display* display = nullptr);
    static void close();

    static Display* display();

    static void movePointer(float x, float y);

    static void injectButtonPress(int btn, const QStringList& modifiers);
    static void injectButtonRelease(int btn, const QStringList& modifiers);
    static void injectButtonPress(int btn);
    static void injectButtonRelease(int btn);

    static void injectKeyPress(const QString& key, const QStringList& modifiers);
    static void injectKeyRelease(const QString& key, const QStringList& modifiers);
    static void injectKeyPress(const QString& key);
    static void injectKeyRelease(const QString& key);

private:
    static Display* disp;
    static bool shouldCloseDisplay;
};

#endif /* XTEST_H */
