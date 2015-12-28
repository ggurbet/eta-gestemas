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

#ifndef GESTURERECOGNIZER_H
#define GESTURERECOGNIZER_H

#include "state.h"

class GestureRecognizerManager;
class Touch;

class GestureRecognizer
{
public:
    GestureRecognizer();
    virtual ~GestureRecognizer();
    virtual bool isEqual(const GestureRecognizer& other) const = 0;
    void touchBeganHandler(const Touch *touch);
    void touchMovedHandler(const Touch *touch);
    void touchEndedHandler(const Touch *touch);
    void setManager(GestureRecognizerManager* manager);

    GestureRecognizer(const GestureRecognizer&) = delete;
    GestureRecognizer& operator=(const GestureRecognizer&) = delete;
protected:
    void setState(const State& state);
    virtual void onTouchBegan(const Touch *touch) = 0;
    virtual void onTouchMoved(const Touch *touch) = 0;
    virtual void onTouchEnded(const Touch *touch) = 0;
    virtual void reset() = 0;
private:
    State m_state;
    GestureRecognizerManager *m_manager;
};

#endif /* GESTURERECOGNIZER_H */
