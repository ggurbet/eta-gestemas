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

#include "touchmanager.h"
#include "gesturerecognizermanager.h"

/**
 * @class TouchManager
 * @brief   Base class for touch manager classes.
 *
 * We can have multiple concrete touch manager classes. Each handles touch
 * management with a different library. Concrete touch managers should be able
 * to accept/reject touches and process and extract touch event info provided by
 * WindowManagerAdapter through WindowManagerAdapterListener.
 */

/**
 * @fn virtual void TouchManager::processTouchEvent(void *data)
 * @brief   Extracts touch info out of @p data.
 *
 * Invokes GestureRecognizerManager methods for each touch event with
 * required parameters. Paramters are extracted from @p data.
 *
 * @param[in] data      touch info ptr sent by window manager
 */

/**
 * @fn void TouchManager::acceptTouch(unsigned long touchId, unsigned long
 *                                      targetId, void* device)
 * @brief Accepts ownership of the touch events of @p touchId.
 *
 * Touch events are not replayed to other clients if the touch is accepted. All
 * touches should be either accepted or rejected in the end. Otherwise it blocks
 * touch input system including pointer emulation. We should accept touches as
 * soon as we recognize a gesture, or reject them in a shortest possible period
 * of time we are sure we could not recognize any gesture.
 *
 * @param[in] touchId        touch id to be accepted
 * @param[in] targetId       window id to accept the touch for
 * @param[in] device         pointer to device object which generated the events
 *                           with @p touchId
 */

/**
 * @fn void TouchManager::rejectTouch(unsigned long touchId,
 *                                      unsigned long targetId, void* device)
 * @brief   Sends all touch events with  @p touchId to other clients.
 *
 * This method replays all grabbed touch events which has the id @p touchId to
 * other clients.
 *
 * @param[in] touchId        touch id to be rejected
 * @param[in] targetId       window id to reject the touch for
 * @param[in] device         pointer to device object which generated the events
 *                           with @p touchId
 */

/**
 * @brief   TouchManager constructor.
 *
 * This constructor initializes its GestureRecognizerManager member to nullptr.
 * TouchManager asserts GestureRecognizerManager is not nullptr. If it is not
 * set to a valid value, application exists.
 */
TouchManager::TouchManager()
    :m_grm(nullptr)
{
}

/**
 * @brief   Sets GestureRecognizerManager.
 *
 * Touch begin/update/end events are forwarded to this gesture recognizer manager.
 * This gesture recognizer manager is also responsible for creating and deleting
 * Touch objects on touch begin and end events respectively.
 *
 * @param[in] grm        GestureRecognizerManager for this TouchManager
 *
 * @see TouchManager::gestureRecognizerManager()
 */
void TouchManager::setGestureRecognizerManager(GestureRecognizerManager* grm)
{
    m_grm = grm;
}

/**
 * @brief   Returns TouchManager's GestureRecognizerManager.
 *
 * @return              GestureRecognizerManager instance which is in charge of
 *                      creating and deleting Touch instances and keeping track
 *                      of them.
 *
 * @see TouchManager::setGestureRecognizerManager()
 */
GestureRecognizerManager* TouchManager::gestureRecognizerManager()
{
    return m_grm;
}
