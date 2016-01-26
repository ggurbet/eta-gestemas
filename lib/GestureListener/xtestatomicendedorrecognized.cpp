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

#include "xtestatomicendedorrecognized.h"
#include "gesturerecognizer.h"
#include "xtestshortcut.h"
#include "utilities.h"
#include <QtCore/QtDebug>

XTestAtomicEndedOrRecognized::XTestAtomicEndedOrRecognized()
    :m_shortcut(nullptr)
{
}
XTestAtomicEndedOrRecognized::~XTestAtomicEndedOrRecognized()
{
    delete m_shortcut;
    m_shortcut = nullptr;
}
void XTestAtomicEndedOrRecognized::setAtomicEndedOrRecognizedShortcut(
                                        const XTestShortcut *shortcut)
{
    m_shortcut = shortcut;
}

void XTestAtomicEndedOrRecognized::onBegan()
{
}

void XTestAtomicEndedOrRecognized::onRecognized()
{
    // Could be a discrete gesture recognizer
    m_shortcut->press();
    m_shortcut->release();
}

void XTestAtomicEndedOrRecognized::onChanged()
{
}

void XTestAtomicEndedOrRecognized::onCanceled()
{
}

void XTestAtomicEndedOrRecognized::onEnded()
{
    // Could be a continues gesture recognizer
    m_shortcut->press();
    m_shortcut->release();
}

void XTestAtomicEndedOrRecognized::onFailed()
{
}

bool XTestAtomicEndedOrRecognized::isEqual(const GestureListener& other) const
{
    (void)other;
    return true;
}
