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
#include "xtestshortcut.h"
#include "xtest.h"

void XTestShortcut::setValue(const QString& value, Type type)
{
    m_type = type;
    if (m_type == Button) {
        bool ok = false;
        int val = value.toInt(&ok, 10);
        if (ok) {
            m_buttonValue = val;
        }
    } else if (m_type == Key) {
        m_keyValue = value;
    }
}

void XTestShortcut::addModifier(const QString& modifier)
{
    m_modifiers.append(modifier);
}

void XTestShortcut::press() const
{
    if (m_type == Key) {
        if (m_modifiers.size() == 0) {
            XTest::injectKeyPress(m_keyValue);
        } else {
            XTest::injectKeyPress(m_keyValue, m_modifiers);
        }
    } else if (m_type == Button) {
        if (m_modifiers.size() == 0) {
            XTest::injectButtonPress(m_buttonValue);
        } else {
            XTest::injectButtonPress(m_buttonValue, m_modifiers);
        }
    }
}

void XTestShortcut::release() const
{
    if (m_type == Key) {
        if (m_modifiers.size() == 0) {
            XTest::injectKeyRelease(m_keyValue);
        } else {
            XTest::injectKeyRelease(m_keyValue, m_modifiers);
        }
    } else if (m_type == Button) {
        if (m_modifiers.size() == 0) {
            XTest::injectButtonRelease(m_buttonValue);
        } else {
            XTest::injectButtonRelease(m_buttonValue, m_modifiers);
        }
    }
}

void XTestShortcut::pressAndRelease() const
{
    if (m_type == Key) {
        if (m_modifiers.size() == 0) {
            XTest::injectKeyPressRelease(m_keyValue);
        } else {
            XTest::injectKeyPressRelease(m_keyValue, m_modifiers);
        }
    } else if (m_type == Button) {
        if (m_modifiers.size() == 0) {
            XTest::injectButtonPressRelease(m_buttonValue);
        } else {
            XTest::injectButtonPressRelease(m_buttonValue, m_modifiers);
        }
    }
}

void XTestShortcut::movePointer(float x, float y) const
{
    XTest::movePointer(x, y);
}
