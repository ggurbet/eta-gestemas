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

#include "xtestatomicbeganorrecognized.h"
#include "gesturerecognizer.h"
#include "xtestshortcut.h"
#include "utilities.h"
#include <QtCore/QtDebug>

XTestAtomicBeganOrRecognized::XTestAtomicBeganOrRecognized()
    :m_shortcut(nullptr)
{
}
XTestAtomicBeganOrRecognized::~XTestAtomicBeganOrRecognized()
{
    delete m_shortcut;
    m_shortcut = nullptr;
}
void XTestAtomicBeganOrRecognized::setAtomicBeganOrRecognizedShortcut(
                                        const XTestShortcut *shortcut)
{
    m_shortcut = shortcut;
}

void XTestAtomicBeganOrRecognized::onBegan()
{
    // Could be a continues gesture recognizer
    m_shortcut->press();
    m_shortcut->release();
}

void XTestAtomicBeganOrRecognized::onRecognized()
{
    // Could be a discrete gesture recognizer
    m_shortcut->press();
    m_shortcut->release();
}

void XTestAtomicBeganOrRecognized::onChanged()
{
}

void XTestAtomicBeganOrRecognized::onCanceled()
{
}

void XTestAtomicBeganOrRecognized::onEnded()
{
}

void XTestAtomicBeganOrRecognized::onFailed()
{
}

bool XTestAtomicBeganOrRecognized::isEqual(const GestureListener& other) const
{
    (void)other;
    return true;
}
