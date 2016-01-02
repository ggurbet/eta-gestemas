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

#include "windowmanageradapterlistener.h"
#include "touchmanager.h"
#include "targetfactory.h"
#include "target.h"
#include "gesturerecognizermanager.h"
#include <QtCore/QtGlobal>

WindowManagerAdapterListener::WindowManagerAdapterListener(TouchManager *tm,
                                TargetFactory *tf)
{
    m_grm = new GestureRecognizerManager;
    Q_CHECK_PTR(m_grm);
    setTouchManager(tm);
    setTargetFactory(tf);
}

WindowManagerAdapterListener::~WindowManagerAdapterListener()
{
    delete m_grm;
    m_grm = nullptr;
}

void WindowManagerAdapterListener::onWindowCreated(unsigned long targetId,
                             const QString& targetName,
                             bool *grabTouches)
{
    Target *target;
    target = m_tf->create(targetId, targetName);
    if (!target) {
        *grabTouches = false;
        return;
    }
    *grabTouches = true;
    m_grm->addTarget(target);
}

void WindowManagerAdapterListener::onWindowDestroyed(unsigned long targetId)
{
    m_grm->removeTarget(targetId);
}

void WindowManagerAdapterListener::onTouchEvent(void *data)
{
    m_tm->processTouchEvent(data);
}

void WindowManagerAdapterListener::setTouchManager(TouchManager *touchManager)
{
    m_tm = touchManager;
    if (m_tm) {
        m_tm->setGestureRecognizerManager(m_grm);
        m_grm->setTouchManager(m_tm);
    }
}

TouchManager* WindowManagerAdapterListener::touchManager()
{
    return m_tm;
}

void WindowManagerAdapterListener::setTargetFactory(TargetFactory* targetFactory)
{
    m_tf = targetFactory;
}

TargetFactory* WindowManagerAdapterListener::targetFactory()
{
    return m_tf;
}
