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

#include "swipegesturerecognizer.h"
#include "touch.h"
#include "utilities.h"
#include <cmath>
#include <QtCore/QTimer>
#include <QtCore/QtDebug>

SwipeGestureRecognizer::SwipeGestureRecognizer()
    :GestureRecognizer(),
     m_numTouchesRequired(2),
     m_maxDuration(500),
     m_minDisplacement(0.0020f),
     m_maxAngle(40.0f * DEGREE_TO_RADIAN),
     m_direction(NoDirection),
     m_minVelocity(0.0f),
     m_noDirection(true),
     m_startTime(0UL),
     m_timer(nullptr)
{
    m_timer = new QTimer(this);
    Q_CHECK_PTR(m_timer);
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this,
            &SwipeGestureRecognizer::onTimeout);
}

bool SwipeGestureRecognizer::isEqual(const GestureRecognizer& other) const
{
    if (!GestureRecognizer::isEqual(other)) return false;

    const SwipeGestureRecognizer *p =
        static_cast<const SwipeGestureRecognizer*>(&other);
    if (m_numTouchesRequired != p->m_numTouchesRequired) return false;
    if (m_maxDuration != p->m_maxDuration) return false;
    if (m_minDisplacement != p->m_minDisplacement) return false;
    if (m_maxAngle != p->m_maxAngle) return false;
    if (m_direction != p->m_direction) return false;

    return true;
}

void SwipeGestureRecognizer::setMaxDuration(int maxDuration)
{
    m_maxDuration = maxDuration;
    m_minVelocity = m_minDisplacement / m_maxDuration;
}

void SwipeGestureRecognizer::setMinDisplacement(float minDisplacement)
{
    m_minDisplacement = minDisplacement;
    m_minVelocity = m_minDisplacement / m_maxDuration;
}

void SwipeGestureRecognizer::reset()
{
    GestureRecognizer::reset();
    if (m_timer->isActive()) {
        m_timer->stop();
    }
}

void SwipeGestureRecognizer::onTouchBegan(const Touch *touch)
{
    if (numTouches() > numTouchesRequired()) {
        if (state() == State::Possible) {
            setState(State::Failed);
        } else {
            ignoreTouch(touch);
        }
        return;
    }

    if (numTouches() == 1) {
        m_noDirection = ((m_direction & Orthogonal) == 0);
        m_startTime = touch->timestamp();
        m_timer->start(maxDuration());
    }
}

void SwipeGestureRecognizer::onTouchMoved(const Touch *touch)
{
    if (numTouches() < numTouchesRequired()) {
        return;
    }

    uint64_t deltaTime = touch->timestamp() - m_startTime;
    if (deltaTime == 0) {
        return;
    }

    float prevCentralX = centralX();
    float prevCentralY = centralY();
    updateCentralPoint();
    float deltaX = centralX()  - prevCentralX;
    float deltaY = centralY()  - prevCentralY;
    float velocityX = deltaX / deltaTime;
    float velocityY = deltaY / deltaTime;
    float deltaSquared = SQUARED_PYTHAGOREAN(deltaY, deltaX);
    float velocitySquared = SQUARED_PYTHAGOREAN(velocityY, velocityX);
    float thresholdSquared = SQUARED(recognitionThreshold());
    float minDisplacementSquared = SQUARED(minDisplacement());
    float minVelocitySquared = SQUARED(m_minVelocity);
    qDebug() << m_minVelocity << " " << minDisplacement() << " " << maxDuration();
    qDebug() << sqrtf(velocitySquared) << " " << sqrtf(deltaSquared) << " " << deltaTime;

    if (m_noDirection) {
        if (deltaSquared >= thresholdSquared
            && velocitySquared >= minVelocitySquared
            && deltaSquared >= minDisplacementSquared) {
            setState(State::Recognized);
        } else {
            float absVelocityX  = fabsf(velocityX);
            float absVelocityY  = fabsf(velocityY);
            if (absVelocityX > absVelocityY) {
                if (deltaX > recognitionThreshold()) {
                    if ((deltaX < 0.0f && (m_direction & Left) == 0)
                        || ((deltaX > 0.0f) && (m_direction & Right) == 0)
                        || fabsf(atanf(deltaY / deltaX)) > m_maxAngle) {
                        setState(State::Failed);
                    } else if (absVelocityX >= m_minVelocity
                               || deltaX >= minDisplacement()) {
                        setState(State::Recognized);
                    }
                }
            }
            else if (absVelocityY > absVelocityX) {
                if (deltaY > recognitionThreshold()) {
                    if ((deltaY < 0.0f && (m_direction & Down) == 0)
                        || ((deltaY > 0.0f) && (m_direction & Up) == 0)
                        || fabsf(atanf(deltaX / deltaY)) > m_maxAngle) {
                        setState(State::Failed);
                    } else if (absVelocityY >= m_minVelocity
                               || deltaY >= minDisplacement()) {
                        setState(State::Recognized);
                    }
                }
            } else if (deltaSquared > thresholdSquared) {
                setState(State::Failed);
            }
        }
    }
}

void SwipeGestureRecognizer::onTouchEnded(const Touch *touch)
{
    (void)touch;
    if (numTouches() < numTouchesRequired()) {
            setState(State::Failed);
    }
}

void SwipeGestureRecognizer::onTimeout()
{
    if (state() == State::Possible) {
        setState(State::Failed);
    }
}
