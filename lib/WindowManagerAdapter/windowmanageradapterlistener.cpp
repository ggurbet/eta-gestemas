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

#include "windowmanageradapterlistener.h"
#include "touchmanager.h"
#include "targetfactory.h"
#include "target.h"
#include "gesturerecognizermanager.h"
#include <QtCore/QtGlobal>

/**
 * @class WindowManagerAdapterListenerInterface
 * @brief   Common interface for window manager adapter listeners.
 *
 * A window manager adapter listener could be a test class to test correspoing
 * listener, or it should forward events to gesture recognition classes.
 * Test listeners should inherit from this class for the test cases.
 * Gesture recognition classes are encapsulated in WindowManagerAdapterListener.
 */

/**
 * @fn void WindowManagerAdapterListenerInterface::onWindowCreated(
 *                           unsigned long targetId,
 *                           const QString& targetName,
 *                           bool *grabTouches)
 *
 * @brief   This method is invoked when a new window opened.
 *
 * @param[in] targetId        id of the created target window
 * @param[in] targetName      app class name of created target window
 * @param[out] grapTouches    flag to grab touches for this window
 */

/**
 * @fn void WindowManagerAdapterListenerInterface::onWindowDestroyed(
 *                           unsigned long targetId)
 *
 * @brief   This method is invoked when a window closed.
 *
 * @param[in] targetId        id of closed window
 */

/**
 * @fn void WindowManagerAdapterListenerInterface::onTouchEvent(
 *                           void *data)
 *
 * @brief   This method is invoked on any touch event.
 *
 * The touch event could be a touch begin/update/end event.
 * The type of the event is available in @p data.
 *
 * @param[in] data        window manager specific touch event data
 */

/**
 * @class WindowManagerAdapterListener
 * @brief   This class is a concrete window manager adapter listener.
 *
 * It forwards window creation events to an instance of TargetFactory,
 * window destruction events to an internally created instance of
 * GestureRecognitionManager and touch begin/update/end events to
 * a derived TouchManager instance.
 */

 /**
  * @brief Constructor of WindowManagerAdapterListener.
  *
  * This constructor creates GestureRecognitionManager internally.
  *
  * @param[in] tm       ptr to touch manager derived class instance
  * @param[in] tf       ptr to target factory instance
  */
WindowManagerAdapterListener::WindowManagerAdapterListener(TouchManager *tm,
                                TargetFactory *tf)
{
    m_grm = new GestureRecognizerManager;
    Q_CHECK_PTR(m_grm);
    setTouchManager(tm);
    setTargetFactory(tf);
}

/**
 * @brief  Destructor of WindowManagerAdapterListener.
 *
 * This destructor deletes GestureRecognizerManager member created by
 * the constructor.
 */
WindowManagerAdapterListener::~WindowManagerAdapterListener()
{
    delete m_grm;
    m_grm = nullptr;
}

/**
 * @brief   Creates targets and add them to the member GestureRecognizerManager.
 *
 * New targets are created through the TargetFactory. If TargetFactory returns
 * nullptr for the created window, @p grabTouches is set to @p false. When the
 * created window is the root or it is a window which has no gesture recognizer
 * to be attached in the config file, TargetFactory returns
 * nullptr for it. Otherwise @p grabTouches is set to true and instantiated
 * target is added to GestureRecognizerManager.
 *
 * @see TargetFactory::create(),
 * WindowManagerAdapterListenerInterface::onWindowCreated(),
 * GestureRecognizerManager::addTarget()
 */
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

/**
 * @brief   Remove target from the member GestureRecognizerManager.
 *
 * @see WindowManagerAdapterListenerInterface::onWindowCreated(),
 * GestureRecognizerManager::removeTarget()
 */
void WindowManagerAdapterListener::onWindowDestroyed(unsigned long targetId)
{
    m_grm->removeTarget(targetId);
}

/**
 * @brief   Passes @p data to TouchManager.
 *
 * @see WindowManagerAdapterListenerInterface::onTouchEvent(),
 * TouchManager::processTouchEvent()
 */
void WindowManagerAdapterListener::onTouchEvent(void *data)
{
    m_tm->processTouchEvent(data);
}

/**
 * @brief   Sets TouchManager member.
 *
 * Also sets @p touchManager's GestureRecognizerManager to internally created
 * instance.

 * @param[in] touchManager        ptr to TouchManager instance
 */
void WindowManagerAdapterListener::setTouchManager(TouchManager *touchManager)
{
    m_tm = touchManager;
    if (m_tm) {
        m_tm->setGestureRecognizerManager(m_grm);
        m_grm->setTouchManager(m_tm);
    }
}

/**
 * @brief   Returns TouchManager ptr.
 */
TouchManager* WindowManagerAdapterListener::touchManager()
{
    return m_tm;
}

/**
 * @brief   Sets TargetFactory member.
 *
 * @param[in] targetFactory       ptr to TargetFactory instance
 */
void WindowManagerAdapterListener::setTargetFactory(TargetFactory* targetFactory)
{
    m_tf = targetFactory;
}

/**
 * @brief   Returns TargetFactory ptr.
 */
TargetFactory* WindowManagerAdapterListener::targetFactory()
{
    return m_tf;
}
