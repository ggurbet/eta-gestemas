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

#include "xtestlongpressrightclick.h"
#include "longpressgesturerecognizer.h"
#include "xtestshortcut.h"
#include "utilities.h"
#include <QtCore/QtDebug>

XTestLongPressRightClick::XTestLongPressRightClick()
    :m_shortcut(nullptr)
{
}
XTestLongPressRightClick::~XTestLongPressRightClick()
{
    delete m_shortcut;
    m_shortcut = nullptr;
}
void XTestLongPressRightClick::setRightClickShortcut(const XTestShortcut *shortcut)
{
    m_shortcut = shortcut;
}

void XTestLongPressRightClick::onBegan()
{
    m_shortcut->press();
    m_shortcut->release();
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
