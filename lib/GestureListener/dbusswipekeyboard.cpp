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

#include "dbusswipekeyboard.h"
#include "swipegesturerecognizer.h"
#include "utilities.h"
#include <QtCore/QtDebug>

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

DBusSwipeKeyboard::DBusSwipeKeyboard()
{
    QDBusConnection conn = QDBusConnection::sessionBus();
    m_interface = new QDBusInterface("org.eta.etakeyboard",
                                     "/EtaKeyboard",
                                     "org.eta.etakeyboard", conn);

}

DBusSwipeKeyboard::~DBusSwipeKeyboard()
{
    m_interface->deleteLater();
}

void DBusSwipeKeyboard::onBegan()
{
}

void DBusSwipeKeyboard::onRecognized()
{
    qDebug() << "Switch recognized";
    if (m_recognizer->cumulativeDeltaY() > 0.0f) {
        m_interface->call(m_hideCommand);
    } else {
        m_interface->call(m_showCommand);
    }
}

void DBusSwipeKeyboard::onChanged()
{
}

void DBusSwipeKeyboard::onCanceled()
{
}

void DBusSwipeKeyboard::onEnded()
{
}

void DBusSwipeKeyboard::onFailed()
{
}

bool DBusSwipeKeyboard::isEqual(const GestureListener& other) const
{
    (void)other;
    return true;
}
