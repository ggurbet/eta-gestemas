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

#ifndef TWOTOUCHGESTURERECOGNIZER_H
#define TWOTOUCHGESTURERECOGNIZER_H

#include "pinchgesturerecognizer.h"

class Touch;

class TwoTouchPinchGestureRecognizer : public PinchGestureRecognizer
{
public:
    TwoTouchPinchGestureRecognizer();
    virtual bool isEqual(const GestureRecognizer& other) const;
    virtual void reset();

    virtual float scale() const
    {return m_scale;}

    virtual float velocity() const
    {return m_velocity;}

protected:
    virtual void onTouchBegan(const Touch *touch);
    virtual void onTouchMoved(const Touch *touch);
    virtual void onTouchEnded(const Touch *touch);
private:
    float m_scale;
    float m_velocity;
    const Touch *m_touch1;
    const Touch *m_touch2;
    float m_distance;
    float m_cumulativeDeltaDistance;
    uint32_t m_timestamp;
};

#endif /* TWOTOUCHGESTURERECOGNIZER_H */
