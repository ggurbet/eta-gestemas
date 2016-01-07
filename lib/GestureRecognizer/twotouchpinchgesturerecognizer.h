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

#include "gesturerecognizer.h"

class Touch;

class TwoTouchPinchGestureRecognizer : public GestureRecognizer
{
public:
    TwoTouchPinchGestureRecognizer();
    virtual bool isEqual(const GestureRecognizer& other) const;
    virtual void reset();

    void setScale(float scale)
    {m_scale = scale;}
    float scale() const
    {return m_scale;}

    void setMaxScale(float maxScale)
    {m_maxScale = maxScale;}
    float maxScale() const
    {return m_maxScale;}

    void setMinScale(float minScale)
    {m_minScale = minScale;}
    float minScale() const
    {return m_minScale;}

    void setAccumulator(int accumulator)
    {m_accumulator = (accumulator > 1) ? accumulator : 1;}
    int accumulator() const
    {return m_accumulator;}
protected:
    virtual void onTouchBegan(const Touch *touch);
    virtual void onTouchMoved(const Touch *prev, const Touch *current);
    virtual void onTouchEnded(const Touch *prev, const Touch *current);
private:
    float m_scale;
    float m_maxScale;
    float m_minScale;
    int m_accumulator;

    const Touch *m_touch1;
    const Touch *m_touch2;
    float m_distanceSquared;
    int m_counter;

};

#endif /* TWOTOUCHGESTURERECOGNIZER_H */
