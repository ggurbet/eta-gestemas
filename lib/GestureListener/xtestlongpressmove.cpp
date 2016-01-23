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

#include "xtestlongpressmove.h"
#include "longpressgesturerecognizer.h"
#include "xtestshortcut.h"
#include "utilities.h"
#include <QtCore/QtDebug>

XTestLongPressMove::XTestLongPressMove()
    :m_shortcut(nullptr)
{
}

XTestLongPressMove::~XTestLongPressMove()
{
    delete m_shortcut;
    m_shortcut = nullptr;
}

void XTestLongPressMove::setMoveShortcut(const XTestShortcut *shortcut)
{
    m_shortcut = shortcut;
}

void XTestLongPressMove::onBegan()
{
    m_shortcut->press();
}

void XTestLongPressMove::onRecognized()
{
}

void XTestLongPressMove::onChanged()
{
    m_shortcut->movePointer(m_recognizer->centralX(),
                            m_recognizer->centralY());
}

void XTestLongPressMove::onCanceled()
{
    m_shortcut->release();
}

void XTestLongPressMove::onEnded()
{
        m_shortcut->release();
}

void XTestLongPressMove::onFailed()
{
}

bool XTestLongPressMove::isEqual(const GestureListener& other) const
{
    (void)other;
    return true;
}
