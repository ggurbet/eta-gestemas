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
 * along with ete-gestemas.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WINDOWMANAGERADAPTER_H
#define WINDOWMANAGERADAPTER_H

#include <QtCore/QObject>
#include "windowmanageradapter_tst.h"

class WindowManagerAdapter : public QObject
{
    Q_OBJECT
public:
    explicit WindowManagerAdapter(QObject *parent = nullptr);
    WindowManagerAdapter(const WindowManagerAdapter &) = delete;
    WindowManagerAdapter &operator=(const WindowManagerAdapter &) = delete;

    void setTester(WindowManagerAdapterTestBase * tester);

    virtual void dispatchEvents() = 0;

protected slots:
    virtual void onNewEvent() = 0;

protected:
    WindowManagerAdapterTestBase *m_tester;
};

#endif
