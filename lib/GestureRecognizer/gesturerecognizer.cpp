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

#include "gesturerecognizer.h"
#include "gesturerecognizermanager.h"
#include "touch.h"
#include <QtCore/QtGlobal>
#include <QtCore/QtDebug>

GestureRecognizer::GestureRecognizer(QObject *parent)
    :QObject(parent), m_manager(nullptr), m_centralX(0.0f), m_centralY(0.0f),
     m_maxAllowableDrift(0.0001f), m_allowSimultaneousRecognition(false)
{
}

void GestureRecognizer::reset()
{
    setCentralX(0.0f);
    setCentralY(0.0f);

    m_touches.clear();

    Q_ASSERT(m_state.isLeaf());
    setState(State::Possible);
}

void GestureRecognizer::touchBeganHandler(const Touch *touch)
{
    const Touch *prev = nullptr;
    prev = findTouch(touch->touchId());
    Q_ASSERT(prev == nullptr);
    prev = touch;
    m_touches.append(prev);
    onTouchBegan(prev);
}

void GestureRecognizer::touchMovedHandler(const Touch *touch)
{
    const Touch *prev = nullptr;
    prev = findTouch(touch->touchId());
    Q_ASSERT(prev != nullptr);
    onTouchMoved(prev, touch);
}

void GestureRecognizer::touchEndedHandler(const Touch *touch)
{
    const Touch *prev = nullptr;
    prev = findTouch(touch->touchId());
    Q_ASSERT(prev != nullptr);
    m_touches.removeAll(prev);
    onTouchEnded(prev, touch);
}

void GestureRecognizer::setManager(GestureRecognizerManager* manager)
{
    m_manager = manager;
}

void GestureRecognizer::setState(const State& newState)
{
    qDebug() << m_state.toString() << "->" << newState.toString();
    Q_ASSERT(m_state.canTransitionTo(newState));
    m_state = newState;

    // TODO: Call GestureListener callbacks here.

    // if (m_state == State::Began) {
    //     qDebug() << "Began";
    // } else if (m_state == State::Changed) {
    //     qDebug() << "Changed";
    // } else if (m_state == State::Ended) {
    //     qDebug() << "Ended";
    // } else if (m_state == State::Recognized) {
    //     qDebug() << "Recognized";
    // } else if (m_state == State::Canceled) {
    //     qDebug() << "Canceled";
    // } else if (m_state == State::Failed) {
    //     qDebug() << "Failed";
    // }

    if (m_state.isLeaf()) {
        m_manager->detachGestureRecognizer(this);
    }

    if (m_allowSimultaneousRecognition
        && (m_state == State::Began || m_state == State::Recognized)) {
        GestureRecognizer *gestureRecognizer = nullptr;
        State state;
        foreach (gestureRecognizer, gestureRecognizersToAbort()) {
            state = gestureRecognizer->m_state;
            if (state == State::Possible) {
                gestureRecognizer->setState(State::Failed);
            } else if (state == State::Began
                       || state == State::Changed) {
                gestureRecognizer->setState(State::Canceled);
            }
        }
    }
}

void GestureRecognizer::updateCentralPoint()
{
    float x = 0.0f;
    float y = 0.0f;
    int size = m_touches.size();

    if (size > 0) {
        const Touch *touch = nullptr;
        foreach (touch, m_touches) {
            x += touch->x();
            y += touch->y();
        }
        if (size == 1) {
            m_centralX = x;
            m_centralY = y;
        } else {
            m_centralX = x / size;
            m_centralY = y / size;
        }
    }
}

void GestureRecognizer::ignoreTouch(const Touch* touch)
{
    m_manager->detachGestureRecognizer(touch, this);
}

const Touch* GestureRecognizer::findTouch(uint32_t touchId)
{
    const Touch *touch = nullptr;
    foreach (touch, m_touches) {
        if (touch->touchId() == touchId) {
            return touch;
        }
    }
    return nullptr;;
}
