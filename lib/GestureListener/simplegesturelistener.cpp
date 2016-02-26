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

#include "simplegesturelistener.h"
#include "gesturerecognizer.h"

/**
 * @class GestureListener
 * @brief   Base class for all gesture listeners.
 *
 * Each concrete gesture listener should implement this interface.
 *
 * @fn bool GestureListener::isEqual(const GestureListener& other) const
 * @brief Compares this listener with @p other.
 *
 * @fn void GestureListener::onBegan()
 * @brief Called when state changed to Began.
 *
 * @fn void GestureListener::onRecognized()
 * @brief Called when state changed to Recognized.
 *
 * @fn void GestureListener::onChanged()
 * @brief Called when state changed to Changed.
 *
 * @fn void GestureListener::onCanceled()
 * @brief Called when state changed to Canceled.
 *
 * @fn void GestureListener::onEnded()
 * @brief Called when state changed to Ended.
 *
 * @fn void GestureListener::onFailed()
 * @brief Called when state changed to Failed.
 */
void SimpleGestureListener::setGestureRecognizer(
                            const GestureRecognizer *recognizer)
{
    m_recognizer = recognizer;
}
