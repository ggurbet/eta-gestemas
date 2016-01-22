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

#include <QtCore/QtDebug>
#include "xtest.h"
#include "xtestlongpressrightclick.h"
#include "longpressgesturerecognizer.h"
#include "utilities.h"

void XTestLongPressRightClick::onBegan()
{
    const char *modifiers[] = {""};
    XTest::injectButton(3, modifiers);
    // XTest::injectKey(XStringToKeysym("Menu"), modifiers);
}

void XTestLongPressRightClick::onRecognized()
{
}

void XTestLongPressRightClick::onChanged()
{
}

void XTestLongPressRightClick::onCanceled()
{
}

void XTestLongPressRightClick::onEnded()
{
}

void XTestLongPressRightClick::onFailed()
{
}

bool XTestLongPressRightClick::isEqual(const GestureListener& other) const
{
    (void)other;
    return true;
}
