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
#include "touch.h"
#include "utilities.h"
#include <QtCore/QtDebug>

PanGestureRecognizer::PanGestureRecognizer()
    :GestureRecognizer(),
     m_maxNumTouchesRequired(2),
     m_minNumTouchesRequired(2),
     m_maxVelocity(20.0f),
     m_minVelocity(0.1f),
     m_velocityX(0.0f),
     m_velocityY(0.0f),
     m_translationX(0.0f),
     m_translationY(0.0f)
{
}

bool PanGestureRecognizer::isEqual(const GestureRecognizer& other) const
{
    const PanGestureRecognizer *p =
        static_cast<const PanGestureRecognizer*>(&other);

    if (m_maxVelocity != p->m_maxVelocity) return false;
    if (m_minVelocity != p->m_minVelocity) return false;
    if (m_maxNumTouchesRequired != p->m_maxNumTouchesRequired) return false;
    if (m_minNumTouchesRequired != p->m_minNumTouchesRequired) return false;
    if (m_maxAllowableDrift != p->m_maxAllowableDrift) return false;
    if (m_allowSimultaneousRecognition !=
        p->m_allowSimultaneousRecognition) return false;

    return true;
}

void PanGestureRecognizer::onTouchBegan(const Touch *touch)
{
    qDebug() << "Pan onTouchBegan";
    // qDebug() << "touchId:" << touch->touchId()
    //          << "x:" << touch->x()
    //          << "y:" << touch->y()
    //          << "resolutionX:" << touch->resolutionX()
    //          << "resolutionY:" << touch->resolutionY()
    //          << "timeStamp:" <<  touch->timeStamp();

    if (numTouches() > maxNumTouchesRequired()) {
        if (state() == State::Possible) {
            setState(State::Failed);
        } else {
            ignoreTouch(touch);
        }
        return;
    }
    if (numTouches() >= minNumTouchesRequired()) {
        updateCentralPoint();
        m_timeStamp = touch->timeStamp();
    }
}

void PanGestureRecognizer::onTouchMoved(const Touch *prev,
                                              const Touch *current)
{
    // const Touch *touch = current;
    qDebug() << "Pan onTouchMoved";
    // qDebug() << "touchId:" << touch->touchId()
    //          << "x:" << touch->x()
    //          << "y:" << touch->y()
    //          << "resolutionX:" << touch->resolutionX()
    //          << "resolutionY:" << touch->resolutionY()
    //          << "timeStamp:" <<  touch->timeStamp();

    if (numTouches() < minNumTouchesRequired()) {
        return;
    }

    float x1 = prev->x();
    float y1 = prev->y();
    float x2 = current->x();
    float y2 = current->y();
    float deltaTranslation = 0.0f;
    uint64_t deltaTime = 0;
    if (state() == State::Possible) {
        m_prevCentralX = centralX();
        m_prevCentralY = centralY();
        updateCentralPoint();
        deltaTime = current->timeStamp() - m_timeStamp;
        deltaTime = (deltaTime == 0) ? 1 : deltaTime;
        m_timeStamp = current->timeStamp();
        if (SQUARED_PYTHAGOREAN(y1, y2, x1, x2) >
            SQUARED(maxAllowableDrift())) {
            deltaTranslation = centralX() - m_prevCentralX;
            m_velocityX = deltaTranslation / deltaTime;
            m_translationX = deltaTranslation;

            deltaTranslation = centralY() - m_prevCentralY;
            m_velocityY = deltaTranslation / deltaTime;
            m_translationY = deltaTranslation;

            setState(State::Began);
        }
    } else if (state() == State::Began || state() == State::Changed) {
        m_prevCentralX = centralX();
        m_prevCentralY = centralY();
        updateCentralPoint();
        deltaTime = current->timeStamp() - m_timeStamp;
        deltaTime = (deltaTime == 0) ? 1 : deltaTime;
        m_timeStamp = current->timeStamp();
        deltaTranslation = centralX() - m_prevCentralX;
        m_velocityX = deltaTranslation / deltaTime;
        m_translationX += deltaTranslation;

        deltaTranslation = centralY() - m_prevCentralY;
        m_velocityY = deltaTranslation / deltaTime;
        m_translationY += deltaTranslation;
        setState(State::Changed);
    }
}

void PanGestureRecognizer::onTouchEnded(const Touch *prev,
                                              const Touch *current)
{
    (void)prev;
    (void)current;
    // const Touch *touch = current;
    qDebug() << "Pan onTouchEnded";
    // qDebug() << "touchId:" << touch->touchId()
    //          << "x:" << touch->x()
    //          << "y:" << touch->y()
    //          << "resolutionX:" << touch->resolutionX()
    //          << "resolutionY:" << touch->resolutionY()
    //          << "timeStamp:" <<  touch->timeStamp();

    if (numTouches() < minNumTouchesRequired()) {
        if (state() == State::Possible) {
            setState(State::Failed);
        } else if (state() == State::Began || state() == State::Changed) {
            setState(State::Ended);
        }
    } else {
        updateCentralPoint();
    }
}

void PanGestureRecognizer::reset()
{
    GestureRecognizer::reset();
}

float PanGestureRecognizer::velocity() const
{
    return sqrtf(SQUARED(m_velocityX) +
                 SQUARED(m_velocityY));
}
