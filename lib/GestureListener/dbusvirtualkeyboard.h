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

#ifndef DBUSVIRTUALSKEYBOARD_H
#define DBUSVIRTUALSKEYBOARD_H

#include "simplegesturelistener.h"
#include <QtCore/QString>

class QDBusInterface;

class DBusVirtualKeyboard : public SimpleGestureListener
{
public:
    DBusVirtualKeyboard();
    virtual ~DBusVirtualKeyboard();
    virtual bool isEqual(const GestureListener& other) const;
    virtual void onBegan();
    virtual void onRecognized();
    virtual void onChanged();
    virtual void onCanceled();
    virtual void onEnded();
    virtual void onFailed();
    void setCommand(const QString& cmd)
    {m_command = cmd;}
private:
    QString m_command;
    QDBusInterface *m_interface;
};

#endif /* DBUSVIRTUALSKEYBOARD_H */
