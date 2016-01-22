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
#include "xtestlongpressmove.h"
#include "longpressgesturerecognizer.h"
#include "utilities.h"

void XTestLongPressMove::onBegan()
{
    XTest::injectKeyPress(XStringToKeysym("Alt_L"));
    XTest::injectLeftButtonPress();
}

void XTestLongPressMove::onRecognized()
{
}

void XTestLongPressMove::onChanged()
{
    XTest::movePointer(m_recognizer->centralX(), m_recognizer->centralY());
}

void XTestLongPressMove::onCanceled()
{
        XTest::injectLeftButtonRelease();
        XTest::injectKeyRelease(XStringToKeysym("Alt_L"));
}

void XTestLongPressMove::onEnded()
{
        XTest::injectLeftButtonRelease();
        XTest::injectKeyRelease(XStringToKeysym("Alt_L"));
}

void XTestLongPressMove::onFailed()
{
}

bool XTestLongPressMove::isEqual(const GestureListener& other) const
{
    (void)other;
    return true;
}