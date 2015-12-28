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

#include "pangesturerecognizer.h"

PanGestureRecognizer::PanGestureRecognizer()
    :m_maxNumTouchesRequired(2),
     m_minNumTouchesRequired(2),
     m_velocity(0.0f),
     m_maxVelocity(20.0f),
     m_minVelocity(0.1f)
{
}

bool PanGestureRecognizer::isEqual(const GestureRecognizer& other) const
{
    const PanGestureRecognizer *p =
        static_cast<const PanGestureRecognizer*>(&other);

    if (m_velocity != p->m_velocity) return false;
    if (m_maxVelocity != p->m_maxVelocity) return false;
    if (m_minVelocity != p->m_minVelocity) return false;
    if (m_maxNumTouchesRequired!= p->m_maxNumTouchesRequired) return false;
    if (m_minNumTouchesRequired!= p->m_minNumTouchesRequired) return false;

    return true;
}

void PanGestureRecognizer::onTouchBegan(const Touch *touch)
{
    
}

void PanGestureRecognizer::onTouchMoved(const Touch *touch)
{
    
}

void PanGestureRecognizer::onTouchEnded(const Touch *touch)
{
    
}

void PanGestureRecognizer::reset()
{
    
}
