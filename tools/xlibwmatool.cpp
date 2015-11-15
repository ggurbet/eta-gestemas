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

#include "xlibwindowmanageradapter.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QtDebug>

class XLibWindowManagerAdapterTester : public WindowManagerAdapterTestBase
{
public:
    virtual void onWindowCreated(unsigned long targetId,
                                 const QString & targetName)
    {
        qDebug() << "Target Created" << "0x" + QString::number(targetId, 16)
                 << " " << targetName;
    }
    virtual void onWindowDestroyed(unsigned long targetId)
    {
        qDebug() << "Target Destroyed" << "0x" + QString::number(targetId, 16);
    }

    virtual void onTouchEvent(void *data)
    {
        qDebug() << "Touch Event";
    }
};

int main(int argc, char *argv[])
{
    qDebug() << "XLibWindowManagerAdapter Test";
    QCoreApplication app(argc, argv);
    XLibWindowManagerAdapterTester tester;
    XLibWindowManagerAdapter windowManagerAdapter(&app);
    windowManagerAdapter.setTester(&tester);
    windowManagerAdapter.dispatchEvents();
    return app.exec();
}
