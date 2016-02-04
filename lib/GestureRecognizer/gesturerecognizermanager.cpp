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
#include "utilities.h"
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
        Target *target = m_targets.takeAt(i);
        if (target) {
            foreach (GestureRecognizer* gr, target->gestureRecognizers()) {
                detachGestureRecognizer(gr);
            }
            delete target;
        }
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
