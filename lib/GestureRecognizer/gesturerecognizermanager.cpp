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

#include "gesturerecognizermanager.h"
#include "gesturerecognizer.h"
#include "touchmanager.h"
#include "touch.h"
#include "target.h"
#include <QtCore/QtGlobal>
#include <QtCore/QtDebug>

GestureRecognizerManager::GestureRecognizerManager()
    :m_touchManager(nullptr)
{
}

GestureRecognizerManager::~GestureRecognizerManager()
{
    while (!m_targets.isEmpty()) {
        delete m_targets.takeFirst();
    }
    while (!m_touches.isEmpty()) {
        delete m_touches.takeFirst();
    }
}

void GestureRecognizerManager::onTouchBegan(uint32_t touchId, float x, float y,
                                           float resolutionX, float resolutionY,
                  uint32_t targetId, void* device, uint64_t timeStamp)
{
    Touch *touch = nullptr;
    Target *target = nullptr;

    touch = findTouch(touchId);
    Q_ASSERT(touch == nullptr);
    touch = new Touch(touchId, targetId, x, y, x, y, resolutionX, resolutionY,
                      device, timeStamp, timeStamp);
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

}

void GestureRecognizerManager::onTouchUpdated(uint32_t touchId,
                                        float x, float y, uint64_t timeStamp)
{
    Touch *prev = nullptr;

    prev = findTouch(touchId);
    Q_ASSERT(prev != nullptr);

    if (prev->ownershipState() == Touch::Rejected) return;

    Touch current(*prev);
    current.setX(x);
    current.setY(y);
    current.setTimeStamp(timeStamp);

    QList<GestureRecognizer*> gestureRecognizers =
        m_gestureRecognizersForTouches.values(touchId);
    GestureRecognizer *gestureRecognizer = nullptr;
    foreach (gestureRecognizer, gestureRecognizers) {
        gestureRecognizer->touchMovedHandler(&current);
    }
    prev->setX(x);
    prev->setY(y);
    prev->setTimeStamp(timeStamp);
    handleTouchOwnership(prev);
    foreach (gestureRecognizer, gestureRecognizers) {
        gestureRecognizer->callListener();
    }
}

void GestureRecognizerManager::onTouchEnded(uint32_t touchId,
                                        float x, float y, uint64_t timeStamp)
{
    Touch *prev = nullptr;
    Target *target = nullptr;

    prev = findTouch(touchId);
    Q_ASSERT(prev != nullptr);

    Touch current(*prev);
    current.setX(x);
    current.setY(y);
    current.setTimeStamp(timeStamp);

    QList<GestureRecognizer*> gestureRecognizers =
        m_gestureRecognizersForTouches.values(touchId);
    GestureRecognizer *gestureRecognizer = nullptr;
    foreach (gestureRecognizer, gestureRecognizers) {
        gestureRecognizer->touchEndedHandler(&current);
    }
    prev->setX(y);
    prev->setY(y);
    prev->setTimeStamp(timeStamp);
    handleTouchOwnership(prev);
    if (prev->ownershipState() == Touch::Deferred) {
        qDebug() << "Rejecting ended defered touch " << touchId;
        rejectTouch(prev);
    }

    foreach (gestureRecognizer, gestureRecognizers) {
        gestureRecognizer->callListener();
    }

    m_touches.removeAll(prev);
    if (m_touches.size() == 0) {
        target = findTarget(prev->targetId());
        gestureRecognizers = target->gestureRecognizers();
        foreach (gestureRecognizer, gestureRecognizers) {
            if (gestureRecognizer->state().isLeaf())
                gestureRecognizer->reset();
        }
    }
    m_gestureRecognizersForTouches.remove(touchId);

    delete prev;
    prev = nullptr;
}

void GestureRecognizerManager::attachGestureRecognizer(const Touch *touch,
                                                       GestureRecognizer* gr)
{
    Q_ASSERT(touch != nullptr);
    m_gestureRecognizersForTouches.insert(touch->touchId(), gr);
}

void GestureRecognizerManager::detachGestureRecognizer(const Touch *touch,
                                                       GestureRecognizer* gr)
{
    Q_ASSERT(touch != nullptr);
    m_gestureRecognizersForTouches.remove(touch->touchId(), gr);
}

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

void GestureRecognizerManager::addTarget(Target* target)
{
    m_targets.append(target);
    GestureRecognizer* gestureRecognizer;
    QList<GestureRecognizer*> gestureRecognizers = target->gestureRecognizers();
    foreach (gestureRecognizer, gestureRecognizers) {
        gestureRecognizer->setManager(this);
    }
}

void GestureRecognizerManager::removeTarget(uint32_t targetId)
{
    int i = 0;
    for(; i < m_targets.size(); ++i) {
        if (m_targets[i]->targetId() == targetId) {
            break;
        }
    }
    if (i < m_targets.size()) {
        delete m_targets.takeAt(i);
    }
}

void GestureRecognizerManager::setTouchManager(TouchManager *touchManager)
{
    m_touchManager = touchManager;
}

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

Touch* GestureRecognizerManager::findTouch(uint32_t touchId)
{
    Touch *touch = nullptr;
    foreach (touch, m_touches) {
        if (touch->touchId() == touchId) {
            return touch;
        }
    }
    return nullptr;
}

Target* GestureRecognizerManager::findTarget(uint32_t targetId)
{
    Target *target = nullptr;
    foreach (target, m_targets) {
        if (target->targetId() == targetId) {
            return target;
        }
    }
    return nullptr;
}

void GestureRecognizerManager::handleTouchOwnership()
{
    Touch *touch = nullptr;
    foreach (touch, m_touches) {
        handleTouchOwnership(touch);
    }
}

void GestureRecognizerManager::handleTouchOwnership(Touch* touch)
{
    QList<GestureRecognizer*> gestureRecognizers =
    m_gestureRecognizersForTouches.values(touch->touchId());
    if (gestureRecognizers.size() == 0) {
        if (touch->ownershipState() == Touch::Deferred) {
            rejectTouch(touch);
        }
    } else {
        GestureRecognizer *gestureRecognizer = nullptr;
        State state;
        foreach (gestureRecognizer, gestureRecognizers) {
            state = gestureRecognizer->state();
            if (!state.isSuccessful()) {
                return;
            }
        }
        if (touch->ownershipState() == Touch::Deferred) {
            acceptTouch(touch);
        }
    }
}
