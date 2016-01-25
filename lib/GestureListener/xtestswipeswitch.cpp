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

#include "xtestswipeswitch.h"
#include "swipegesturerecognizer.h"
#include "xtestshortcut.h"
#include "utilities.h"
#include <QtCore/QtDebug>

XTestSwipeSwitch::XTestSwipeSwitch()
    :m_switchRightShortcut(nullptr),
     m_switchLeftShortcut(nullptr)
{
}

XTestSwipeSwitch::~XTestSwipeSwitch()
{
    delete m_switchRightShortcut;
    delete m_switchLeftShortcut;
    m_switchRightShortcut = nullptr;
    m_switchLeftShortcut = nullptr;
}

void XTestSwipeSwitch::onBegan()
{
}

void XTestSwipeSwitch::onRecognized()
{
    qDebug() << "Switch recognized";
    if (m_recognizer->cumulativeDeltaX() > 0.0f) {
        m_switchRightShortcut->press();
        m_switchRightShortcut->release();
    } else {
        m_switchLeftShortcut->press();
        m_switchLeftShortcut->release();
    }
}

void  XTestSwipeSwitch::setSwitchRightShortcut(const XTestShortcut *shortcut)
{
    m_switchRightShortcut = shortcut;
}

void  XTestSwipeSwitch::setSwitchLeftShortcut(const XTestShortcut *shortcut)
{
    m_switchLeftShortcut = shortcut;
}

void XTestSwipeSwitch::onChanged()
{
}

void XTestSwipeSwitch::onCanceled()
{
}

void XTestSwipeSwitch::onEnded()
{
}

void XTestSwipeSwitch::onFailed()
{
}

bool XTestSwipeSwitch::isEqual(const GestureListener& other) const
{
    (void)other;
    return true;
}
