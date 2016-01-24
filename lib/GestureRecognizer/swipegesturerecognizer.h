
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

#ifndef SWIPEGESTURERECOGNIZER_H
#define SWIPEGESTURERECOGNIZER_H

#include "gesturerecognizer.h"

class Touch;
class QTimer;

class SwipeGestureRecognizer : public GestureRecognizer
{
public:
    enum Direction {
        NoDirection = 0,
        Left        = 1 << 0,
        Right       = 1 << 1,
        Up          = 1 << 2,
        Down        = 1 << 3,
        Horizontal  = Left | Right,
        Vertical    = Up | Down,
        Orthogonal  = Horizontal | Vertical
    };

    SwipeGestureRecognizer();
    virtual bool isEqual(const GestureRecognizer& other) const;
    virtual void reset();

    void setNumTouchesRequired(int numTouchesRequired)
    {m_numTouchesRequired = numTouchesRequired;}
    int numTouchesRequired() const
    {return m_numTouchesRequired;}

    void setMaxDuration(int maxDuration);
    int maxDuration() const
    {return m_maxDuration;}

    void setMinDisplacement(float minDisplacement);
    float minDisplacement() const
    {return m_minDisplacement;}

    void setMaxAngle(float maxAngle)
    {m_maxAngle = maxAngle * DEGREE_TO_RADIAN;}
    float maxAngle() const
    {return m_maxAngle * RADIAN_TO_DEGREE;}

    void setDirection(Direction direction)
    {m_direction = direction;}
    Direction direction() const
    {return m_direction;}

    float cumulativeDeltaX() const
    {return m_cumulativeDeltaX;}
    float cumulativeDeltaY() const
    {return m_cumulativeDeltaY;}

protected:
    virtual void onTouchBegan(const Touch *touch);
    virtual void onTouchMoved(const Touch *touch);
    virtual void onTouchEnded(const Touch *touch);
private slots:
    void onTimeout();
private:
    static constexpr float DEGREE_TO_RADIAN = 0.0174533f;
    static constexpr float RADIAN_TO_DEGREE = 57.2958f;
    int m_numTouchesRequired;
    int m_maxDuration;
    float m_minDisplacement;
    float m_maxAngle;
    Direction m_direction;

    float m_cumulativeDeltaX;
    float m_cumulativeDeltaY;
    float m_minVelocity;
    bool m_noDirection;
    uint64_t m_startTime;
    QTimer *m_timer;
};

#endif /* SWIPEGESTURERECOGNIZER_H */
