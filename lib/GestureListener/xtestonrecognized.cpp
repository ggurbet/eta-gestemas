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

#include "xtestonrecognized.h"
#include "gesturerecognizer.h"
#include "xtestshortcut.h"
#include "utilities.h"
#include <QtCore/QtDebug>

XTestOnRecognized::XTestOnRecognized()
    :m_shortcut(nullptr)
{
}
XTestOnRecognized::~XTestOnRecognized()
{
    delete m_shortcut;
    m_shortcut = nullptr;
}
void XTestOnRecognized::setOnRecognizedShortcut(
                        const XTestShortcut *shortcut)
{
    m_shortcut = shortcut;
}

void XTestOnRecognized::onBegan()
{
}

void XTestOnRecognized::onRecognized()
{
    m_shortcut->pressAndRelease();
}

void XTestOnRecognized::onChanged()
{
}

void XTestOnRecognized::onCanceled()
{
}

void XTestOnRecognized::onEnded()
{
}

void XTestOnRecognized::onFailed()
{
}

bool XTestOnRecognized::isEqual(const GestureListener& other) const
{
    (void)other;
    return true;
}
