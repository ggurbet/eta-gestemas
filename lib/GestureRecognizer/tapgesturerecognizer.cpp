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
#include "touch.h"
#include "utilities.h"
#include <QtCore/QTimer>
#include <QtCore/QtDebug>

TapGestureRecognizer::TapGestureRecognizer()
    :GestureRecognizer(),
     m_numTouchesRequired(2),
     m_numTapsRequired(1),
     m_maxTapDelay(400),
     m_maxTapDuration(1000),
     m_numTouchesRequiredReached(false),
     m_tapCounter(0),
     m_timer(nullptr)
{
    m_timer = new QTimer(this);
    Q_CHECK_PTR(m_timer);
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this,
            &TapGestureRecognizer::onTimeout);
}

bool TapGestureRecognizer::isEqual(const GestureRecognizer& other) const
{
    if (!GestureRecognizer::isEqual(other)) return false;

    const TapGestureRecognizer *p =
        static_cast<const TapGestureRecognizer*>(&other);
    if (m_numTouchesRequired != p->m_numTouchesRequired) return false;
    if (m_numTapsRequired != p->m_numTapsRequired) return false;
    if (m_maxTapDelay != p->m_maxTapDelay) return false;
    if (m_maxTapDuration != p->m_maxTapDuration) return false;
    if (m_maxTapDistance != p->m_maxTapDistance) return false;

    return true;
}

void TapGestureRecognizer::onTouchBegan(const Touch *touch)
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
        m_timer->start(maxTapDuration());
    }

    if (numTapsRequired() > 1) {
        if (m_tapCounter == 0) {
            Point p;
            p.x = touch->computedX();
            p.y = touch->computedY();
            m_initialTouchPoints.append(p);
        } else {
            bool found = false;
            float deltaX = 0.0f;
            float deltaY = 0.0f;
            foreach (const Point& p, m_initialTouchPoints) {
                deltaX = p.x - touch->computedX();
                deltaY = p.y - touch->computedY();
                if (SQUARED_PYTHAGOREAN(deltaY, deltaX) <=
                    SQUARED(maxTapDistance())) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                setState(State::Failed);
                return;
            }
        }
    }

    if (numTouches() == numTouchesRequired()) {
        m_numTouchesRequiredReached = true;
        updateCentralPoint();
    }
}

void TapGestureRecognizer::onTouchMoved(const Touch *touch)
{
    const Touch *t = touch;
    if ((SQUARED(t->cumulativeDeltaX()) + SQUARED(t->cumulativeDeltaY())) >
        SQUARED(recognitionThreshold()) && state() == State::Possible) {
        setState(State::Failed);
    }

}

void TapGestureRecognizer::onTouchEnded(const Touch *touch)
{
    (void)touch;
    if (!m_numTouchesRequiredReached) {
        setState(State::Failed);
    } else if (numTouches() == 0) {
        m_numTouchesRequiredReached = false;
        ++m_tapCounter;
        if (m_timer->isActive()) {
            m_timer->stop();
        }
        if (m_tapCounter == numTapsRequired()) {
            setState(State::Recognized);
        } else {
            m_timer->start(maxTapDelay());
        }
    }
}

void TapGestureRecognizer::reset()
{
    GestureRecognizer::reset();
    m_numTouchesRequiredReached = false;
    m_tapCounter = 0;
    if (m_timer->isActive()) {
        m_timer->stop();
    }
    m_initialTouchPoints.clear();
}

void TapGestureRecognizer::onTimeout()
{
    if (state() == State::Possible) {
        setState(State::Failed);
        handleTouchOwnership();
        callListener();
    }
}
