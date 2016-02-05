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

#include "gesturerecognizermanager.h"
#include "gesturerecognizer.h"
#include "touchmanager.h"
#include "touch.h"
#include "target.h"
#include "utilities.h"
#include <QtCore/QtGlobal>
#include <QtCore/QtDebug>

/**
 * @class GestureRecognizerManager
 * @brief   Central class for gesture recognition.
 *
 * This class is responsible for creating and deleting Touch objects and deleting
 * Target objects.

 * When it receives touch began event from touch manager, it creates a touch
 * object, and deletes the touch object when it receives the touch ended event.
 * When it receives window destroyed event from WindowManagerAdapter through
 * WindowManagerAdapterListener, it deletes destroyed window's target object
 * representation.
 */

/**
 * @brief   Trivial constructor.
 */
GestureRecognizerManager::GestureRecognizerManager()
    :m_touchManager(nullptr)
{
}

/**
 * @brief Deletes all added targets and active touches, if there are any.
 */
GestureRecognizerManager::~GestureRecognizerManager()
{
    while (!m_targets.isEmpty()) {
        delete m_targets.takeFirst();
    }
    while (!m_touches.isEmpty()) {
        delete m_touches.takeFirst();
    }
}

/**
 * @brief   Creates a touch object.
 *
 * This method finds out the target on which this touch is active and registers
 * target's gesture recognizers with the touch. Created touch object is forwarded
 * to all gesture recognizers' touch began handler method, which might change
 * gesture recognizer state. Finally gesture listener callbacks are called for
 * each gesture recognizer of this touch, in case their state might have changed.
 *
 * @param[in] touchId        touch id
 * @param[in] x              x value of the touch event
 * @param[in] y              y value of the touch event
 * @param[in] minX           possible min x value
 * @param[in] minY           possible min y value
 * @param[in] maxX           possible max x value
 * @param[in] maxY           possible max y value
 * @param[in] targetId       window id on which touch is active
 * @param[in] device         device generated this touch
 * @param[in] timestamp      timestamp of the touch event
 *
 * @see GestureRecognizer::touchBeganHandler(),
 * GestureRecognizer::callListener()
 */
void GestureRecognizerManager::onTouchBegan(uint32_t touchId, float x, float y,
                            float minX, float minY, float maxX, float maxY,
                            uint32_t targetId, void* device, uint64_t timestamp)
{
    Touch *touch = nullptr;
    Target *target = nullptr;

    touch = findTouch(touchId);
    Q_ASSERT(touch == nullptr);
    touch = new Touch(touchId, targetId, x, y, minX, minY,
                      maxX, maxY, device, timestamp);

    Q_CHECK_PTR(touch);
    m_touches.append(touch);

    target = findTarget(targetId);
    Q_ASSERT(target != nullptr);

    QList<GestureRecognizer*> gestureRecognizers = target->gestureRecognizers();
    GestureRecognizer *gestureRecognizer = nullptr;
    State state;
    foreach (gestureRecognizer, gestureRecognizers) {
        state = gestureRecognizer->state();
        if (!state.isLeaf()) {
            attachGestureRecognizer(touch, gestureRecognizer);
        }
    }
    gestureRecognizers =
        m_gestureRecognizersForTouches.values(touchId);
    foreach (gestureRecognizer, gestureRecognizers) {
        gestureRecognizer->touchBeganHandler(touch);
    }
    handleTouchOwnership(touch);
    foreach (gestureRecognizer, gestureRecognizers) {
        gestureRecognizer->callListener();
    }

    qDebug() << "Began";
    printStates();
}

/**
 * @brief   Updates touch object.
 *
 * If touch count is one, it could be a pointer emulation. If the touch is
 * neither accepted or rejected for a period of time, and its position change is
 * significant enough, we reject it not to block pointer emulation. This method
 * also samples the touch events. Touch events are handled with a certain time
 * period. Please see configuration file for sampling and pointer emulation
 * constants. Each handled touch event updates touch object which is forwarded to
 * all gesture recognizers' touch update handler, which might change gesture
 * recognizers' state. Finally gesture listener callbacks are called for each
 * gesture recognizer, in case their state might have changed.
 *
 * @param[in] touchId        touch id
 * @param[in] x              x value of the touch event
 * @param[in] y              y value of the touch event
 * @param[in] timestamp      timestamp of the touch event
 *
 * @see GestureRecognizer::touchUpdateHandler(),
 * GestureRecognizer::callListener(),
 * GestureRecognizer::samplingPeriod,
 * GestureRecognizer::pointerEmulationRate,
 * GestureRecognizer::pointerEmulationDistance
 */
void GestureRecognizerManager::onTouchUpdated(uint32_t touchId,
                                        float x, float y, uint64_t timestamp)
{
    Touch *t = nullptr;

    t = findTouch(touchId);
    Q_ASSERT(t != nullptr);

    if (t->ownershipState() == Touch::Rejected) {
        return;
    }

    if (numTouches() == 1
        && t->ownershipState() == Touch::Deferred
        && timestamp - t->startTime() >
        GestureRecognizer::samplingPeriod *
        GestureRecognizer::pointerEmulationRate
        && SQUARED_PYTHAGOREAN(t->cumulativeDeltaY(),
                               t->cumulativeDeltaX()) >
        SQUARED(GestureRecognizer::pointerEmulationDistance)) {
            rejectTouch(t);
            return;
    }

    t->computeX(x - t->previousX());
    t->computeY(y - t->previousY());
    t->setPreviousX(x);
    t->setPreviousY(y);

    float deltaTime = timestamp - t->timestamp();
    if (deltaTime < GestureRecognizer::samplingPeriod) {
        return;
    }

    t->setTimestamp(timestamp);
    QList<GestureRecognizer*> gestureRecognizers =
        m_gestureRecognizersForTouches.values(touchId);
    GestureRecognizer *gestureRecognizer = nullptr;
    foreach (gestureRecognizer, gestureRecognizers) {
        gestureRecognizer->touchMovedHandler(t);
    }
    handleTouchOwnership(t);
    foreach (gestureRecognizer, gestureRecognizers) {
        gestureRecognizer->callListener();
    }

    qDebug() << "Updated";
    printStates();
}

/**
 * @brief   Deletes touch object.
 *
 * This method update touch objects and forwards it to each gesture recognizer's
 * touch ended handler. Calls gesture listener callbacks in case touch ended
 * handler might have changed gesture recognizer state. It resets gesture
 * recognizers if they require and finally deletes touch object with the id @p
 * touchId.
 *
 * @param[in] touchId        touch id
 * @param[in] x              x value of the touch event
 * @param[in] y              y value of the touch event
 * @param[in] timestamp      timestamp of the touch event
 *
 * @see GestureRecognizer::touchEndedHandler(),
 * GestureRecognizer::callListener()
 */
void GestureRecognizerManager::onTouchEnded(uint32_t touchId,
                                        float x, float y, uint64_t timestamp)
{
    Touch *touch = nullptr;
    Target *target = nullptr;

    touch = findTouch(touchId);
    Q_ASSERT(touch != nullptr);

    touch->computeX(x - touch->previousX());
    touch->computeY(y - touch->previousY());
    touch->setPreviousX(x);
    touch->setPreviousY(y);
    touch->setTimestamp(timestamp);

    QList<GestureRecognizer*> gestureRecognizers =
        m_gestureRecognizersForTouches.values(touchId);
    GestureRecognizer *gestureRecognizer = nullptr;
    foreach (gestureRecognizer, gestureRecognizers) {
        gestureRecognizer->touchEndedHandler(touch);
    }
    handleTouchOwnership(touch);
    if (touch->ownershipState() == Touch::Deferred) {
        qDebug() << "Rejecting ended defered touch " << touchId;
        rejectTouch(touch);
    }

    foreach (gestureRecognizer, gestureRecognizers) {
        gestureRecognizer->callListener();
    }

    qDebug() << "Ended";
    printStates();

    m_touches.removeAll(touch);
    if (m_touches.size() == 0) {
        target = findTarget(touch->targetId());
        if (target) {
            gestureRecognizers = target->gestureRecognizers();
            foreach (gestureRecognizer, gestureRecognizers) {
                if (gestureRecognizer->state().isLeaf())
                    gestureRecognizer->reset();
            }
        }
    }
    m_gestureRecognizersForTouches.remove(touchId);

    delete touch;
    touch = nullptr;
}

/**
 * @brief   Subscribes @p gr with @p touch.
 *
 * State of gesture recognizer @p gr is affacted by @p touch's events once it is
 * attached.
 *
 * @param[in] touch        changes in @p touch determines gesture recognizer
                           state
 * @param[in] gr           gesture recognizer to be affected by @p touch
 *
 * @see GestureRecognizerManager::detachGestureRecognizer()
 */
void GestureRecognizerManager::attachGestureRecognizer(const Touch *touch,
                                                       GestureRecognizer* gr)
{
    Q_ASSERT(touch != nullptr);
    m_gestureRecognizersForTouches.insert(touch->touchId(), gr);
}

/**
 * @brief   @gr no longer affected by @p touch.
 *
 * State of gesture recognizer @p gr is no longer affacted by @p touch's events
 * once it is detached.
 *
 * @param[in] touch       touch from which gesture recognizer to be detached
 * state.
 *
 * @param[in] gr           gesture recognizer to detach from @p touch
 *
 * @see GestureRecognizerManager::attachGestureRecognizer()
 */
void GestureRecognizerManager::detachGestureRecognizer(const Touch *touch,
                                                       GestureRecognizer* gr)
{
    Q_ASSERT(touch != nullptr);
    m_gestureRecognizersForTouches.remove(touch->touchId(), gr);
}

/**
 * @brief   @gr no longer affected by any touch currently active.
 *
 * State of gesture recognizer @p gr is no longer affacted by any active touch.
 *
 * @param[in] gr           gesture recognizer to detach from all active touches.
 *
 * @see GestureRecognizerManager::attachGestureRecognizer(),
 * GestureRecognizerManager::detachGestureRecognizer()
 */
void GestureRecognizerManager::detachGestureRecognizer(GestureRecognizer* gr)
{
    QHash<uint32_t, GestureRecognizer*>::iterator i =
        m_gestureRecognizersForTouches.begin();
    while (i != m_gestureRecognizersForTouches.end()) {
        if (i.value() == gr) {
            i = m_gestureRecognizersForTouches.erase(i);
        } else {
            ++i;
        }
    }
}

/**
 * @brief   Add a target (i.e. an open window with gesture recognizers).
 *
 * @p target is appended to target list and this gesture recognizer manager is
 * set to be the manager of @p target's gesture recognizers.
 *
 * @param[in] target        target to be added
 *
 * @see GestureRecognizerManager::removeTarget()
 */
void GestureRecognizerManager::addTarget(Target* target)
{
    m_targets.append(target);
    GestureRecognizer* gestureRecognizer;
    QList<GestureRecognizer*> gestureRecognizers = target->gestureRecognizers();
    foreach (gestureRecognizer, gestureRecognizers) {
        gestureRecognizer->setManager(this);
    }
}

/**
 * @brief   Destroy target with @p targetId.
 *
 * All gesture recognizers of the target identified with @p targetId
 * is detached from all active touches and the target object is destroyed.
 *
 * @param[in] targetId        id of a target
 *
 * @see GestureRecognizerManager::addTarget()
 */
void GestureRecognizerManager::removeTarget(uint32_t targetId)
{
    int i = 0;
    for(; i < m_targets.size(); ++i) {
        if (m_targets[i]->targetId() == targetId) {
            break;
        }
    }
    if (i < m_targets.size()) {
        Target *target = m_targets.takeAt(i);
        if (target) {
            foreach (GestureRecognizer* gr, target->gestureRecognizers()) {
                detachGestureRecognizer(gr);
            }
            delete target;
        }
    }
}

/**
 * @brief   Trivial touch manager setter.
 */
void GestureRecognizerManager::setTouchManager(TouchManager *touchManager)
{
    m_touchManager = touchManager;
}

/**
 * @brief   Trivial touch manager getter.
 */
TouchManager* GestureRecognizerManager::touchManager()
{
    return m_touchManager;
}

void GestureRecognizerManager::acceptTouch(Touch *t)
{
    t->setOwnershipState(Touch::Accepted);
    m_touchManager->acceptTouch(t->touchId(), t->targetId(), t->device());
}

void GestureRecognizerManager::rejectTouch(Touch *t)
{
    t->setOwnershipState(Touch::Rejected);
    m_touchManager->rejectTouch(t->touchId(), t->targetId(), t->device());
}

Touch* GestureRecognizerManager::findTouch(uint32_t touchId) const
{
    Touch *touch = nullptr;
    foreach (touch, m_touches) {
        if (touch->touchId() == touchId) {
            return touch;
        }
    }
    return nullptr;
}

Target* GestureRecognizerManager::findTarget(uint32_t targetId) const
{
    Target *target = nullptr;
    foreach (target, m_targets) {
        if (target->targetId() == targetId) {
            return target;
        }
    }
    return nullptr;
}

/**
 * @brief   Accepts or rejects defered touches.
 *
 * This method goes through all active touches and gesture recognizers attached
 * to them. If all gesture recognizers have been detached for an active defered
 * touch, the touch is rejected. If any gesture recognizer's state is successful
 * (i.e. Recognized, Began, Changed, Ended) for an active defered touch, the
 * touch is accepted. A gesture recognizer is detached from active touches when
 * the recognizer is at a leaf state (i.e. Recognized, Canceled, Ended, Failed)
 * or it is ignored by the recognizer.
 */
void GestureRecognizerManager::handleTouchOwnership()
{
    Touch *touch = nullptr;
    foreach (touch, m_touches) {
        handleTouchOwnership(touch);
    }
}

void GestureRecognizerManager::handleTouchOwnership(Touch* touch)
{
    if (touch->ownershipState() != Touch::Deferred) {
        return;
    }

    QList<GestureRecognizer*> gestureRecognizers =
    m_gestureRecognizersForTouches.values(touch->touchId());
    if (gestureRecognizers.size() == 0) {
        rejectTouch(touch);
    } else {
        GestureRecognizer *gestureRecognizer = nullptr;
        State state;
        foreach (gestureRecognizer, gestureRecognizers) {
            state = gestureRecognizer->state();
            if (state.isSuccessful()) {
                acceptTouch(touch);
                break;
            }
        }
    }
}

void GestureRecognizerManager::printStates() const
{
    const Touch *touch = nullptr;
    const Target *target = nullptr;
    QList<GestureRecognizer*> recognizers;
    int i = 0;
    foreach (touch, m_touches) {
        target = findTarget(touch->targetId());
        QDebug debug = qDebug();
        if (target) {
            recognizers = target->gestureRecognizers();
            for (i = 0; i < recognizers.size(); ++i) {
                debug << recognizers[i]->state().toString();
            }
        }
    }
}
