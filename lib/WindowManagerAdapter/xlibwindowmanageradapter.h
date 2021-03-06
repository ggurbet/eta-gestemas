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

#ifndef XLIBWINDOWMANAGERADAPTER_H
#define XLIBWINDOWMANAGERADAPTER_H

#include "windowmanageradapter.h"


class XLibWindowManagerAdapterPrivate;

class XLibWindowManagerAdapter : public WindowManagerAdapter
{
    Q_OBJECT
    friend class XLibWindowManagerAdapterPrivate;
public:
    explicit XLibWindowManagerAdapter(QObject *parent = nullptr);
    virtual ~XLibWindowManagerAdapter();
    virtual void dispatchEvents();

    void* display();

protected slots:
    virtual void onNewEvent();

private:
    XLibWindowManagerAdapterPrivate *d_ptr;
};

#endif
