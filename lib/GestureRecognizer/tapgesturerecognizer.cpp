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

#include "tapgesturerecognizer.h"

TapGestureRecognizer::TapGestureRecognizer()
    :m_numTouchesRequired(2)
{
}

bool TapGestureRecognizer::isEqual(const GestureRecognizer& other) const
{
    const TapGestureRecognizer *p =
        static_cast<const TapGestureRecognizer*>(&other);

    if (m_numTouchesRequired != p->m_numTouchesRequired) return false;

    return true;
}

void TapGestureRecognizer::onTouchBegan(const Touch *touch)
{
    
}

void TapGestureRecognizer::onTouchMoved(const Touch *touch)
{
    
}

void TapGestureRecognizer::onTouchEnded(const Touch *touch)
{
    
}

void TapGestureRecognizer::reset()
{
    
}
