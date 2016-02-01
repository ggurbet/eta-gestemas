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

#include "xtestonbegan.h"
#include "gesturerecognizer.h"
#include "xtestshortcut.h"
#include "utilities.h"
#include <QtCore/QtDebug>

XTestOnBegan::XTestOnBegan()
    :m_shortcut(nullptr)
{
}
XTestOnBegan::~XTestOnBegan()
{
    delete m_shortcut;
    m_shortcut = nullptr;
}
void XTestOnBegan::setOnBeganShortcut(
                        const XTestShortcut *shortcut)
{
    m_shortcut = shortcut;
}

void XTestOnBegan::onBegan()
{
    m_shortcut->press();
}

void XTestOnBegan::onRecognized()
{
}

void XTestOnBegan::onChanged()
{
}

void XTestOnBegan::onCanceled()
{
    m_shortcut->release();
}

void XTestOnBegan::onEnded()
{
    m_shortcut->release();
}

void XTestOnBegan::onFailed()
{
}

bool XTestOnBegan::isEqual(const GestureListener& other) const
{
    (void)other;
    return true;
}
