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

#ifndef WINDOWMANAGERADAPTERLISTENER_H
#define WINDOWMANAGERADAPTERLISTENER_H

#include "windowmanageradapterlistenerinterface.h"

class TouchManager;
class GestureRecognizerManager;
class TargetFactory;

class WindowManagerAdapterListener :
    public WindowManagerAdapterListenerInterface
{
public:
    WindowManagerAdapterListener(TouchManager *tm = nullptr,
                                 TargetFactory *tf = nullptr);
    virtual ~WindowManagerAdapterListener();
    virtual void onWindowCreated(unsigned long targetId,
                                 const QString& targetName,
                                 bool *grabTouches);
    virtual void onWindowDestroyed(unsigned long targetId);
    virtual void onTouchEvent(void *data);

    void setTouchManager(TouchManager *touchManager);
    TouchManager* touchManager();

    void setTargetFactory(TargetFactory* targetFactory);
    TargetFactory* targetFactory();
protected:
    TouchManager *m_tm;
    TargetFactory *m_tf;
    GestureRecognizerManager *m_grm;
};

#endif /* WINDOWMANAGERADAPTERLISTENER_H */
