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

class TwoTouchPinchGestureRecognizer : public GestureRecognizer
{
public:
    TwoTouchPinchGestureRecognizer();
    virtual bool isEqual(const GestureRecognizer& other) const;

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
protected:
    virtual void onTouchBegan(const Touch *touch);
    virtual void onTouchMoved(const Touch *touch);
    virtual void onTouchEnded(const Touch *touch);
    virtual void reset();
private:
    float m_scale;
    float m_maxScale;
    float m_minScale;
};

#endif /* TWOTOUCHGESTURERECOGNIZER_H */
