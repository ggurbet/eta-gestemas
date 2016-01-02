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

#ifndef TOUCHMANAGER_H
#define TOUCHMANAGER_H

class GestureRecognizerManager;

class TouchManager
{
public:
    TouchManager();
    virtual ~TouchManager() = default;
    virtual void processTouchEvent(void *data) = 0;
    virtual void acceptTouch(unsigned long touchId, unsigned long targetId,
                             void* device) = 0;
    virtual void rejectTouch(unsigned long touchId, unsigned long targetId,
                             void* device) = 0;

    void setGestureRecognizerManager(GestureRecognizerManager* grm);
    GestureRecognizerManager* gestureRecognizerManager();

    TouchManager(const TouchManager&) = delete;
    TouchManager& operator=(const TouchManager&) = delete;
protected:
    GestureRecognizerManager* m_grm;
};

#endif /* TOUCHMANAGER_H */
