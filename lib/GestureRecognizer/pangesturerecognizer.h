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

#ifndef PANGESTURERECOGNIZER_H
#define PANGESTURERECOGNIZER_H

#include "gesturerecognizer.h"

class PanGestureRecognizer : public GestureRecognizer
{
public:
    PanGestureRecognizer();
    virtual bool isEqual(const GestureRecognizer& other) const;

    void setMaxNumTouchesRequired(int maxNumTouchesRequired)
    {m_maxNumTouchesRequired = maxNumTouchesRequired;}
    int maxNumTouchesRequired() const
    {return m_maxNumTouchesRequired;}

    void setMinNumTouchesRequired(int minNumTouchesRequired)
    {m_minNumTouchesRequired = minNumTouchesRequired;}
    int minNumTouchesRequired() const
    {return m_minNumTouchesRequired;}

    void setVelocity(float velocity)
    {m_velocity = velocity;}
    float velocity() const
    {return m_velocity;}

    void setMaxVelocity(float maxVelocity)
    {m_maxVelocity = maxVelocity;}
    float maxVelocity() const
    {return m_maxVelocity;}

    void setMinVelocity(float minVelocity)
    {m_minVelocity = minVelocity;}
    float minVelocity() const
    {return m_minVelocity;}
protected:
    virtual void onTouchBegan(const Touch *touch);
    virtual void onTouchMoved(const Touch *touch);
    virtual void onTouchEnded(const Touch *touch);
    virtual void reset();
private:
    int m_maxNumTouchesRequired;
    int m_minNumTouchesRequired;
    float m_velocity;
    float m_maxVelocity;
    float m_minVelocity;
};

#endif /* PANGESTURERECOGNIZER_H */
