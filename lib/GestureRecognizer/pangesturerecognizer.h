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
    virtual void reset();

    void setMaxNumTouchesRequired(int maxNumTouchesRequired)
    {m_maxNumTouchesRequired = maxNumTouchesRequired;}
    int maxNumTouchesRequired() const
    {return m_maxNumTouchesRequired;}

    void setMinNumTouchesRequired(int minNumTouchesRequired)
    {m_minNumTouchesRequired = minNumTouchesRequired;}
    int minNumTouchesRequired() const
    {return m_minNumTouchesRequired;}

    float velocity() const;

    float velocityX() const
    {return m_velocityX;}
    float velocityY() const
    {return m_velocityY;}

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
    virtual void onTouchMoved(const Touch *prev, const Touch *current);
    virtual void onTouchEnded(const Touch *prev, const Touch *current);
private:
    int m_maxNumTouchesRequired;
    int m_minNumTouchesRequired;
    float m_maxVelocity;
    float m_minVelocity;

    float m_velocityX;
    float m_velocityY;
    float m_translationX;
    float m_translationY;
    float m_prevCentralX;
    float m_prevCentralY;
    uint64_t m_timeStamp;
};

#endif /* PANGESTURERECOGNIZER_H */
