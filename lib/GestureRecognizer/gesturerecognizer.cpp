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

#include "gesturerecognizer.h"
#include "gesturelistener.h"
#include "gesturerecognizermanager.h"
#include "touch.h"
#include "utilities.h"

#include <QtCore/QtGlobal>
#include <QtCore/QtDebug>
#include <cfloat>

/**
 * @class GestureRecognizer
 * @brief   Base class for all gesture recognizers.
 *
 * All gesture recognizers extend this abstract class. They should implement
 * GestureRecognizer::onTouchBegan(), GestureRecognizer::onTouchMoved(),
 * GestureRecognizer::onTouchEnded(), GestureRecognizer::reset() and
 * GestureRecognizer::isEqual() methods.
 */

/**
 * @brief Touch event sampling period, default is 80 ms.
 *
 * @see GestureRecognizerManager::onTouchUpdated()
 */
uint64_t GestureRecognizer::samplingPeriod = 80;

/**
 * @brief Pointer emulation rate.
 *
 * GestureRecognizerManager checks for pointer emulation in (samplingPeriod x
 * pointerEmulationRate) miliseconds after a touch begins. If there is a single
 * active touch and the touch has moved at least pointerEmulationDistance px, the
 * touch is rejected, so that we don't block pointer emulation. Default is 10.
 *
 * @see GestureRecognizerManager::onTouchUpdated()
 */
uint32_t GestureRecognizer::pointerEmulationRate = 10;

/**
 * @brief Single touch mimimum movement for pointer emulation.
 *
 * Minimum movement for a single touch to be considered a pointer emulation.
 * Default is 10 px.
 *
 * @see GestureRecognizerManager::onTouchUpdated()
 */
float GestureRecognizer::pointerEmulationDistance = 10.0f;

/**
 *  @fn void GestureRecognizer::onTouchBegan(const Touch *touch)
 *  @brief Main gesture recognition logic on touch began event.
 *
 *  Concrete recognizers should impelement this method.
 *
 *  @fn void GestureRecognizer::onTouchMoved(const Touch *touch)
 *  @brief Main gesture recognition logic on touch moved event.
 *
 *  Concrete recognizers should impelement this method.
 *
 *  @fn void GestureRecognizer::onTouchEnded(const Touch *touch)
 *  @brief Main gesture recognition logic on touch ended event.
 *
 *  Concrete recognizers should impelement this method.
 *
 * @fn const State& GestureRecognizer::state() const
 * @brief Returns state.
 *
 * @fn void GestureRecognizer::setRecognitionThreshold(float threshold)
 * @brief Sets recognitionThreshold.
 *
 * @fn float GestureRecognizer::recognitionThreshold() const
 * @brief Returns recognitionThreshold.
 *
 * @fn void GestureRecognizer::setAllowSimultaneousRecognition(bool b)
 * @brief Sets if recognizers supports simultaneous recognition.
 *
 * @fn bool GestureRecognizer::allowsSimultaneousRecognition() const
 * @brief Returns if recognizers supports simultaneous recognition.
 *
 * @fn void GestureRecognizer::setGestureRecognizersToAbort(const QList<GestureRecognizer*>& list)
 * @brief Sets abort list to be aborted when this recognizer recognizes its gesture.
 *
 * @fn const GestureRecognizer::QList<GestureRecognizer*>& gestureRecognizersToAbort() const
 * @brief Returns abort list to be aborted when this recognizer recognizes its gesture.
 *
 * @fn int GestureRecognizer::numTouches() const
 * @brief Returns number of touches this recognizer keeps track of.
 *
 * @fn void GestureRecognizer::setTopMargin(float margin)
 * @brief Sets top margin for this recognizer.
 *
 * @fn void GestureRecognizer::setBottomMargin(float margin)
 * @brief Sets bottom margin for this recognizer.
 *
 * @fn void GestureRecognizer::setLeftMargin(float margin)
 * @brief Sets left margin for this recognizer.
 *
 * @fn void GestureRecognizer::setRightMargin(float margin)
 * @brief Sets right margin for this recognizer.
 *
 * @fn void GestureRecognizer::setCentralX(float x)
 * @brief Sets central x coordinate.
 *
 * @fn float GestureRecognizer::centralX() const
 * @brief Returns central x coordinate.
 *
 * @fn void GestureRecognizer::setCentralY(float y)
 * @brief Sets central y coordinate.
 *
 * @fn float GestureRecognizer::centralY() const
 * @brief Returns central y coordinate.
 *
 * @fn void GestureRecognizer::setId(int id)
 * @brief Sets recognizer id.
 *
 * @fn int GestureRecognizer::id() const
 * @brief Returns recognizer id.
 *
 * @fn uint32_t GestureRecognizer::targetId() const
 * @brief   Returns the targetId this recognizer operates on.
 */

/**
 * @brief   Constructor sets initial values of attributes.
 * @param[in] parent        QObject parent this recognizer
 */
GestureRecognizer::GestureRecognizer(QObject *parent)
    :QObject(parent), m_manager(nullptr),
     m_topMargin(FLT_MAX), m_bottomMargin(FLT_MAX),
     m_leftMargin(FLT_MAX), m_rightMargin(FLT_MAX),
     m_centralX(0.0f), m_centralY(0.0f),
     m_recognitionThreshold(1.0f), m_allowSimultaneousRecognition(false),
     m_listener(nullptr), m_id(0)
{
}

/**
 * @brief   Destructor deletes gesture listener instance.
 */
GestureRecognizer::~GestureRecognizer()
{
    delete m_listener;
    m_listener = nullptr;
}

/**
 * @brief   Compares this recognizer with @p other.
 *
 * This method member-wise compares this and @p other recognizer. Concrete
 * recognizers first call base method, then implement their own comparison
 * logic.
 *
 * @param[in] other        recognizer to compare
 * @return                 true if equal, else false.
 */
bool GestureRecognizer::isEqual(const GestureRecognizer& other) const
{
    if (m_recognitionThreshold != other.m_recognitionThreshold) {
        return false;
    }
    if (m_allowSimultaneousRecognition != other.m_allowSimultaneousRecognition) {
        return false;
    }

    if (m_listener && other.m_listener) {
        if (!m_listener->isEqual(*other.m_listener)) {
            return false;
        }
    }

    return true;
}

/**
 * @brief   Resets this recognizer for next recognition cycle.
 *
 * This method resets the internals of recognizer to initial values for next
 * recognition. When all active touches are ended, GestureRecognizerManager
 * resets all recognizers using this method. Concrete recognizers should first
 * call base reset method, then implement their specific reset logic.
 *
 * @see GestureRecognizerManager::onTouchEnded()
 */
void GestureRecognizer::reset()
{
    setCentralX(0.0f);
    setCentralY(0.0f);

    m_touches.clear();

    Q_ASSERT(m_state.isLeaf());

    setState(State::Possible);
    m_states.clear();
}

/**
 * @brief Accepts touch object from GestureRecognizerManager::onTouchBegan()
 *
 * This method performs some edge checking for current touch object and forwards
 * it to GestureRecognizer::onTouchBegan().
 *
 * @param[in] touch        touch object
 */
void GestureRecognizer::touchBeganHandler(const Touch *touch)
{
    const Touch *t = nullptr;
    t = findTouch(touch->touchId());
    Q_ASSERT(t == nullptr);
    t = touch;
    m_touches.append(t);
    m_targetId = t->targetId();
    float x = t->computedX();
    float y = t->computedY();
    float minX = t->minimumX();
    float minY = t->minimumY();
    float maxX = t->maximumX();
    float maxY = t->maximumY();
    if (CHECK_RANGE(x, minX, m_leftMargin + minX)
        && CHECK_RANGE(x, maxX - m_rightMargin, maxX)
        && CHECK_RANGE(y, minY, m_topMargin + minY)
        && CHECK_RANGE(y, maxY - m_bottomMargin, maxY)) {
        onTouchBegan(t);
    } else {
        if (state() == State::Possible) {
            setState(State::Failed);
        } else if (state() == State::Began
                   || state() == State::Changed) {
            setState(State::Canceled);
        }
    }
}

/**
 * @brief Accepts touch object from GestureRecognizerManager::onTouchUpdated()
 *
 * This method forwards @p touch to GestureRecognizer::onTouchMoved()
 *
 * @param[in] touch        touch object
 */
void GestureRecognizer::touchMovedHandler(const Touch *touch)
{
    const Touch *t = nullptr;
    t = findTouch(touch->touchId());
    Q_ASSERT(t != nullptr);
    onTouchMoved(touch);
}

/**
 * @brief Accepts touch object from GestureRecognizerManager::onTouchEnded()
 *
 * This method forwards @p touch to GestureRecognizer::onTouchEnded()
 *
 * @param[in] touch        touch object
 */
void GestureRecognizer::touchEndedHandler(const Touch *touch)
{
    const Touch *t = nullptr;
    t = findTouch(touch->touchId());
    Q_ASSERT(t != nullptr);
    m_touches.removeAll(t);
    onTouchEnded(touch);
}

/**
 * @brief   Calls listener methods according to recognizer state.
 */
void GestureRecognizer::callListener()
{
    if (!m_listener) {
        return;
    }

    State state;
    while (!m_states.isEmpty()) {
        state = m_states.takeFirst();
        if (state == State::Began) {
            m_listener->onBegan();
        } else if (state == State::Changed) {
            m_listener->onChanged();
        } else if (state == State::Ended) {
            m_listener->onEnded();
        } else if (state == State::Recognized) {
            m_listener->onRecognized();
        } else if (state == State::Canceled) {
            m_listener->onCanceled();
        } else if (state == State::Failed) {
            m_listener->onFailed();
        }
    }
}

/**
 * @brief   Calls GestureRecognizerManager::handleTouchOwnership().
 */
void GestureRecognizer::handleTouchOwnership() const
{
    m_manager->handleTouchOwnership();
}

/**
 * @brief   Trivial gesture recognizer manager setter.
 */
void GestureRecognizer::setManager(GestureRecognizerManager* manager)
{
    m_manager = manager;
}

void GestureRecognizer::setState(const State& newState)
{
    // qDebug() << m_state.toString() << "->" << newState.toString();
    Q_ASSERT(m_state.canTransitionTo(newState));
    m_state = newState;
    m_states.append(m_state);

    if (m_state.isLeaf()) {
        m_manager->detachGestureRecognizer(this);
    }

    if (m_state == State::Began || m_state == State::Recognized) {
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
            x += touch->computedX();
            y += touch->computedY();
        }
        if (size == 1) {
            m_centralX = x;
            m_centralY = y;
        } else {
            m_centralX = x / size;
            m_centralY = y / size;
        }
    } else {
        m_centralX = 0.0f;
        m_centralY = 0.0f;
    }
}

void GestureRecognizer::ignoreTouch(const Touch* touch)
{
    m_manager->detachGestureRecognizer(touch, this);
    m_touches.removeAll(touch);
}

const Touch* GestureRecognizer::findTouch(uint32_t touchId)
{
    const Touch *touch = nullptr;
    foreach (touch, m_touches) {
        if (touch->touchId() == touchId) {
            return touch;
        }
    }
    return nullptr;
}

/**
 * @brief   Trivial gesture listener setter.
 */
void GestureRecognizer::setGestureListener(GestureListener *listener)
{
    m_listener = listener;
}

/**
 * @brief   Trivial gesture listener getter.
 */
const GestureListener* GestureRecognizer::listener() const
{
    return m_listener;
}
