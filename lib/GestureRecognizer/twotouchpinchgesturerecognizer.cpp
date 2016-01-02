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

#include "twotouchpinchgesturerecognizer.h"
#include "touch.h"
#include <QtCore/QtDebug>

TwoTouchPinchGestureRecognizer::TwoTouchPinchGestureRecognizer()
     :GestureRecognizer(),
     m_scale(0.0f),
     m_maxScale(3.0f),
     m_minScale(1.2f)
{
}

bool TwoTouchPinchGestureRecognizer::isEqual(const GestureRecognizer& other) const
{
    const TwoTouchPinchGestureRecognizer *p =
        static_cast<const TwoTouchPinchGestureRecognizer*>(&other);

    if (m_scale != p->m_scale) return false;
    if (m_maxScale != p->m_maxScale) return false;
    if (m_minScale != p->m_minScale) return false;
    if (m_maxAllowableDrift != p->m_maxAllowableDrift) return false;
    if (m_allowSimultaneousRecognition !=
        p->m_allowSimultaneousRecognition) return false;

    return true;
}

void TwoTouchPinchGestureRecognizer::onTouchBegan(const Touch *touch)
{
    qDebug() << "TwoTouchPinch onTouchBegan";
    qDebug() << "touchId:" << touch->touchId()
             << "x:" << touch->x()
             << "y:" << touch->y()
             << "resolutionX:" << touch->resolutionX()
             << "resolutionY:" << touch->resolutionY()
             << "timeStamp:" <<  touch->timeStamp();

}

void TwoTouchPinchGestureRecognizer::onTouchMoved(const Touch *prev,
                                              const Touch *current)
{
    const Touch *touch = current;
    qDebug() << "TwoTouchPinch onTouchMoved";
    qDebug() << "touchId:" << touch->touchId()
             << "x:" << touch->x()
             << "y:" << touch->y()
             << "resolutionX:" << touch->resolutionX()
             << "resolutionY:" << touch->resolutionY()
             << "timeStamp:" <<  touch->timeStamp();
}

void TwoTouchPinchGestureRecognizer::onTouchEnded(const Touch *prev,
                                              const Touch *current)
{
    const Touch *touch = current;
    qDebug() << "TwoTouchPinch onTouchEnded";
    qDebug() << "touchId:" << touch->touchId()
             << "x:" << touch->x()
             << "y:" << touch->y()
             << "resolutionX:" << touch->resolutionX()
             << "resolutionY:" << touch->resolutionY()
             << "timeStamp:" <<  touch->timeStamp();
}

void TwoTouchPinchGestureRecognizer::reset()
{
    
}
