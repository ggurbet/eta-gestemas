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
#include "windowmanageradapter.h"

/**
 * @class WindowManagerAdapter
 * @brief   Base class for window manager adapters.
 *
 * Window manager adapters may use different libraries to manage
 * the same window manager, or even more they may have to deal with
 * different window managers. This class defines the required pure
 * virtual methods with a simple testing feature.
 */

/**
 * @fn void WindowManagerAdapter::onNewEvent()
 * @brief   Dispatches new events to corresponding classes.
 *
 * After a call to dispatchEvents(), this method dispatches new events
 * to TargetFactory, TouchManager and GestureRecognizerManager instances.
 * - Window created events are directed to TargetFactory.
 * - Window destroyed events are directed to GestureRecognizerManager.
 * - Touch events are directed to TouchManager.
 */

 /**
 * @fn void WindowManagerAdapter::dispatchEvents()
 * @brief Starts event dispatching.
 *
 * First dispatches window created events for already open windows, then
 * listen for window creation, window destruction and touch events
 * by the help of onNewEvent(). Events are directed to corresponding
 * class instances in onNewEvent() method.
 */

WindowManagerAdapter::WindowManagerAdapter(QObject *parent)
    :QObject(parent), m_listener(nullptr) {}

/**
 * @brief   Set @p tester member.
 *
 * Tester's virtual methods get called on every event that a window manager
 * adapter should be interested in. These events are namely window creation,
 * window destruction and touch events on a specific window.
 * @param[in] tester        used for testing the class. If not set, it is null
 */
void WindowManagerAdapter::setListener(WindowManagerAdapterListenerInterface * listener)
{
    m_listener = listener;
}
