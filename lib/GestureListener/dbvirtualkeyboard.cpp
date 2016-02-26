/* Copyright (C) 2015-2016 Gökhan Karabulut <gokhan.karabulut@tubitak.gov.tr>
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

#include "dbusvirtualkeyboard.h"
#include "gesturerecognizer.h"
#include "utilities.h"
#include <QtCore/QtDebug>

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

DBusVirtualKeyboard::DBusVirtualKeyboard()
{
    QDBusConnection conn = QDBusConnection::sessionBus();
    m_interface = new QDBusInterface("org.eta.etakeyboard",
                                     "/EtaKeyboard",
                                     "org.eta.etakeyboard", conn);
    m_command = "toggle";

}

DBusVirtualKeyboard::~DBusVirtualKeyboard()
{
    m_interface->deleteLater();
}

void DBusVirtualKeyboard::onBegan()
{
}

void DBusVirtualKeyboard::onRecognized()
{
    m_interface->call(m_command);
}

void DBusVirtualKeyboard::onChanged()
{
}

void DBusVirtualKeyboard::onCanceled()
{
}

void DBusVirtualKeyboard::onEnded()
{
}

void DBusVirtualKeyboard::onFailed()
{
}

bool DBusVirtualKeyboard::isEqual(const GestureListener& other) const
{
    (void)other;
    return true;
}
