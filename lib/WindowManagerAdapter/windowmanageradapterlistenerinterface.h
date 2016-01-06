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

#ifndef WINDOWMANAGERADAPTERLISTENERINTERFACE_H
#define WINDOWMANAGERADAPTERLISTENERINTERFACE_H

class QString;

class WindowManagerAdapterListenerInterface
{
public:
    virtual ~WindowManagerAdapterListenerInterface() = default;
    virtual void onWindowCreated(unsigned long targetId,
                                 const QString& targetName,
                                 bool *grabTouches) = 0;
    virtual void onWindowDestroyed(unsigned long targetId) = 0;
    virtual void onTouchEvent(void *data) = 0;
};

#endif /* WINDOWMANAGERADAPTERLISTENERINTERFACE_H */
