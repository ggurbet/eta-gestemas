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

#include "longpressgesturerecognizer.h"

LongPressGestureRecognizer::LongPressGestureRecognizer()
    :m_numTouchesRequired(1),
     m_minPressDuration(900),
     m_maxAllowableDrift(0.0001f)
{
}

bool LongPressGestureRecognizer::isEqual(const GestureRecognizer& other) const
{
    const LongPressGestureRecognizer *p =
        static_cast<const LongPressGestureRecognizer*>(&other);

    if (m_numTouchesRequired != p->m_numTouchesRequired) return false;
    if (m_minPressDuration != p->m_minPressDuration) return false;
    if (m_maxAllowableDrift != p->m_maxAllowableDrift) return false;

    return true;
}

void LongPressGestureRecognizer::onTouchBegan(const Touch *touch)
{
    
}

void LongPressGestureRecognizer::onTouchMoved(const Touch *touch)
{
    
}

void LongPressGestureRecognizer::onTouchEnded(const Touch *touch)
{
    
}

void LongPressGestureRecognizer::reset()
{
    
}
