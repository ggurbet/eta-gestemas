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

protected:
    virtual void onTouchBegan(const Touch *touch);
    virtual void onTouchMoved(const Touch *touch);
    virtual void onTouchEnded(const Touch *touch);
private:
    int m_maxNumTouchesRequired;
    int m_minNumTouchesRequired;

    float m_velocityX;
    float m_velocityY;
    float m_translationX;
    float m_translationY;
    uint64_t m_timestamp;
};

#endif /* PANGESTURERECOGNIZER_H */
