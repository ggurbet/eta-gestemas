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

#ifndef DBUSSWIPESKEYBOARD_H
#define DBUSSWIPESKEYBOARD_H

#include "swipelistener.h"
#include <QtCore/QString>

class QDBusInterface;

class DBusSwipeKeyboard : public SwipeListener
{
public:
    DBusSwipeKeyboard();
    virtual ~DBusSwipeKeyboard();
    virtual bool isEqual(const GestureListener& other) const;
    virtual void onBegan();
    virtual void onRecognized();
    virtual void onChanged();
    virtual void onCanceled();
    virtual void onEnded();
    virtual void onFailed();
    void setShowCommand(const QString& cmd)
    {m_showCommand = cmd;}
    void setHideCommand(const QString& cmd)
    {m_hideCommand = cmd;}
private:
    QString m_showCommand;
    QString m_hideCommand;
    QDBusInterface *m_interface;
};

#endif /* DBUSSWIPESKEYBOARD_H */
