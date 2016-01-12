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

#ifndef GESTURERECOGNIZERMANAGER_H
#define GESTURERECOGNIZERMANAGER_H

#include <QtCore/QList>
#include <QtCore/QMultiHash>
#include <cstdint>

class TouchManager;
class Target;
class Touch;
class GestureRecognizer;

class GestureRecognizerManager
{
public:
    GestureRecognizerManager();
    ~GestureRecognizerManager();

    void onTouchBegan(uint32_t touchId, float x, float y,
                      float resolutionX, float resolutionY,
                    uint32_t targetId, void* device, uint64_t timeStamp);
    void onTouchUpdated(uint32_t touchId, float x, float y, uint64_t timeStamp);
    void onTouchEnded(uint32_t touchId, float x, float y, uint64_t timeStamp);

    void detachGestureRecognizer(const Touch *touch, GestureRecognizer* gr);
    void detachGestureRecognizer(GestureRecognizer* gr);
    void handleTouchOwnership();

    void addTarget(Target* target);
    void removeTarget(uint32_t targetId);

    void setTouchManager(TouchManager *touchManager);
    TouchManager* touchManager();


    int numTouches() const
    {return m_touches.size();}

    int numTargets() const
    {return m_targets.size();}

    GestureRecognizerManager(const GestureRecognizerManager&) = delete;
    GestureRecognizerManager& operator=(const GestureRecognizerManager&) = delete;

private:
    void acceptTouch(Touch *t);
    void rejectTouch(Touch *t);
    Touch* findTouch(uint32_t touchId) const;
    Target* findTarget(uint32_t targetId) const;
    void handleTouchOwnership(Touch* touch);
    void attachGestureRecognizer(const Touch *touch, GestureRecognizer* gr);
    void printStates() const;

    TouchManager *m_touchManager;
    QList<Target*> m_targets;
    QList<Touch*> m_touches;
    QMultiHash<uint32_t, GestureRecognizer*> m_gestureRecognizersForTouches;
};

#endif /* GESTURERECOGNIZERMANAGER_H */
