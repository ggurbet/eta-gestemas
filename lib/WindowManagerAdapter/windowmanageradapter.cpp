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
 * virtual methods.
 */

/**
 * @fn void WindowManagerAdapter::onNewEvent()
 * @brief   Dispatches new events to WindowManagerAdapterListenerInterface.
 *
 * After a call to dispatchEvents(), this method sends new events
 * to a listener of type WindowManagerAdapterListenerInterface.
 * WindowManagerAdapterListener dispatches the following events
 * to the following class instances for gesture recognition.
 * - Window created events are directed to TargetFactory.
 * - Window destroyed events are directed to GestureRecognizerManager.
 * - Touch events are directed to TouchManager.
 * WindowManagerAdapterListenerInterface can also be used for test purposes.
 *
 * @see WindowManagerAdapterListener
 */

 /**
 * @fn void WindowManagerAdapter::dispatchEvents()
 * @brief Starts event dispatching.
 *
 * First dispatches window created events for already open windows, then
 * listens for window creation, window destruction and touch events
 * by the help of WindowManagerAdapter::onNewEvent().
 *
 * @see WindowManagerAdapter::setListener()
 */

WindowManagerAdapter::WindowManagerAdapter(QObject *parent)
    :QObject(parent), m_listener(nullptr) {}

/**
 * @brief   Set @p listener member.
 *
 * Listener's corresponding virtual methods are called on every event that a
 * window manager adapter should be interested in. These events are namely window
 * creation, window destruction and touch events on a specific window.
 * @p listener can be used for testing WindowManagerAdapter or it should dispatch
 * events to corresponding classes for gesture recognition.
 *
 * @param[in] listener        events are passed to @p listener, default nullptr
 *
 * @see WindowManagerAdapter::onNewEvent()
 */
void WindowManagerAdapter::setListener(
                    WindowManagerAdapterListenerInterface * listener)
{
    m_listener = listener;
}
