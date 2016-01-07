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
#include "utilities.h"
#include <cmath>
#include <QtCore/QtDebug>

TwoTouchPinchGestureRecognizer::TwoTouchPinchGestureRecognizer()
     :GestureRecognizer(),
      m_scale(1.0f),
      m_maxScale(3.0f),
      m_minScale(1.2f),
      m_accumulator(1),
      m_touch1(nullptr),
      m_touch2(nullptr),
      m_distanceSquared(1.0f),
      m_counter(m_accumulator)
{
}

bool TwoTouchPinchGestureRecognizer::isEqual(const GestureRecognizer& other) const
{
    const TwoTouchPinchGestureRecognizer *p =
        static_cast<const TwoTouchPinchGestureRecognizer*>(&other);

    if (m_maxScale != p->m_maxScale) return false;
    if (m_minScale != p->m_minScale) return false;
    if (m_maxAllowableDrift != p->m_maxAllowableDrift) return false;
    if (m_allowSimultaneousRecognition !=
        p->m_allowSimultaneousRecognition) return false;
    if (m_accumulator != p->m_accumulator) return false;

    return true;
}

void TwoTouchPinchGestureRecognizer::onTouchBegan(const Touch *touch)
{
    // qDebug() << "TwoTouchPinch onTouchBegan";
    // qDebug() << "touchId:" << touch->touchId()
    //          << "x:" << touch->x()
    //          << "y:" << touch->y()
    //          << "resolutionX:" << touch->resolutionX()
    //          << "resolutionY:" << touch->resolutionY()
    //          << "timeStamp:" <<  touch->timeStamp();

    if (numTouches() > 2) {
        if (state() == State::Possible) {
            setState(State::Failed);
        } else {
            ignoreTouch(touch);
        }
        return;
    }

    if (numTouches() == 1) {
        m_touch1 = touch;
    } else { //numTouches() == 2
        m_touch2 = touch;
        float x1 = m_touch1->x();
        float y1 = m_touch1->y();
        float x2 = m_touch2->x();
        float y2 = m_touch2->y();
        m_distanceSquared =
            SQUARED_PYTHAGOREAN(y1, y2, x1, x2);
        // Just in case
        if (m_distanceSquared == 0.0f) {
            m_distanceSquared = 1.0f;
        }
    }
}

void TwoTouchPinchGestureRecognizer::onTouchMoved(const Touch *prev,
                                              const Touch *current)
{
    (void)prev;
    // const Touch *touch = current;
    // qDebug() << "TwoTouchPinch onTouchMoved";
    // qDebug() << "touchId:" << touch->touchId()
    //          << "x:" << touch->x()
    //          << "y:" << touch->y()
    //          << "resolutionX:" << touch->resolutionX()
    //          << "resolutionY:" << touch->resolutionY()
    //          << "timeStamp:" <<  touch->timeStamp();

    float x1 = current->startX();
    float y1 = current->startY();
    float x2 = current->x();
    float y2 = current->y();
    if (numTouches() < 2) {
        if ((state() == State::Possible)
            && (SQUARED_PYTHAGOREAN(y1, y2, x1, x2) >=
                SQUARED(maxAllowableDrift()))) {
            setState(State::Failed);
        }
        return;
    }

    float currentDistanceSquared = 0.0f;
    float reciprocalScale = 0.0f;
    float scale = 0.0f;
    x1 = m_touch1->x();
    y1 = m_touch1->y();
    x2 = m_touch2->x();
    y2 = m_touch2->y();
    currentDistanceSquared = SQUARED_PYTHAGOREAN(y1, y2, x1, x2);

    if (m_accumulator == 1) {
        m_scale = sqrtf(currentDistanceSquared / m_distanceSquared);
        m_distanceSquared = currentDistanceSquared;
    } else {
        if (m_counter == m_accumulator) {
            m_scale = 0.0f;
        }
        if (m_counter > 0) {
            m_scale += sqrtf(currentDistanceSquared / m_distanceSquared);
            m_distanceSquared = currentDistanceSquared;
            --m_counter;
            return;
        }
        m_counter = m_accumulator;
        m_scale /= m_accumulator;
    }

    if (m_scale >= 1.0f) {
        scale = m_scale;
    } else {
        reciprocalScale = m_scale;
    }

    if ((scale >= m_minScale
         && scale <= m_maxScale)
        || (reciprocalScale <= 1.0f / m_minScale
            && reciprocalScale >= 1.0f / m_maxScale)) {
        if (state() == State::Possible) {
            updateCentralPoint();
            setState(State::Began);
        } else if (state() == State::Began || state() == State::Changed) {
            updateCentralPoint();
            setState(State::Changed);
        }
    }
}

void TwoTouchPinchGestureRecognizer::onTouchEnded(const Touch *prev,
                                              const Touch *current)
{
    (void)current;
    // const Touch *touch = current;
    // qDebug() << "TwoTouchPinch onTouchEnded";
    // qDebug() << "touchId:" << touch->touchId()
    //          << "x:" << touch->x()
    //          << "y:" << touch->y()
    //          << "resolutionX:" << touch->resolutionX()
    //          << "resolutionY:" << touch->resolutionY()
    //          << "timeStamp:" <<  touch->timeStamp();

    if (numTouches() == 0) {
        if (state() == State::Began || state() == State::Changed) {
            updateCentralPoint();
            setState(State::Ended);
        } else if (state() == State::Possible) {
            setState(State::Failed);
        }
    } else { // numTouches() == 1
        if (m_touch1 == prev) {
            m_touch1 = m_touch2;
        }
        m_touch2 = nullptr;
    }
}

void TwoTouchPinchGestureRecognizer::reset()
{
    GestureRecognizer::reset();
    m_touch1 = m_touch2 = nullptr;
    m_scale = 1.0f;
    m_distanceSquared = 1.0f;
    m_counter = m_accumulator;
}
