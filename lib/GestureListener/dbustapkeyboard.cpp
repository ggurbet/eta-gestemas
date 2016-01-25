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

#include "dbustapkeyboard.h"
#include "swipegesturerecognizer.h"
#include "utilities.h"
#include <QtCore/QtDebug>

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

DBusTapKeyboard::DBusTapKeyboard()
{
    QDBusConnection conn = QDBusConnection::sessionBus();
    m_interface = new QDBusInterface("org.eta.etakeyboard",
                                     "/EtaKeyboard",
                                     "org.eta.etakeyboard", conn);
    m_toggleCommand = "toggle";

}

DBusTapKeyboard::~DBusTapKeyboard()
{
    m_interface->deleteLater();
}

void DBusTapKeyboard::onBegan()
{
}

void DBusTapKeyboard::onRecognized()
{
    m_interface->call(m_toggleCommand);
}

void DBusTapKeyboard::onChanged()
{
}

void DBusTapKeyboard::onCanceled()
{
}

void DBusTapKeyboard::onEnded()
{
}

void DBusTapKeyboard::onFailed()
{
}

bool DBusTapKeyboard::isEqual(const GestureListener& other) const
{
    (void)other;
    return true;
}
