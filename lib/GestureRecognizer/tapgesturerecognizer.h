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

#ifndef TAPGESTURERECOGNIZER_H
#define TAPGESTURERECOGNIZER_H

#include "gesturerecognizer.h"
#include <QtCore/QList>

class Touch;
class QTimer;

class TapGestureRecognizer : public GestureRecognizer
{
    struct Point
    {
        float x;
        float y;
    };

public:
    TapGestureRecognizer();
    virtual bool isEqual(const GestureRecognizer& other) const;
    virtual void reset();

    void setNumTouchesRequired(int numTouchesRequired)
    {m_numTouchesRequired = numTouchesRequired;}
    int numTouchesRequired() const
    {return m_numTouchesRequired;}

    void setNumTapsRequired(int numTapsRequired)
    {m_numTapsRequired = numTapsRequired;}
    int numTapsRequired() const
    {return m_numTapsRequired;}

    void setMaxTapDelay(int maxTapDelay)
    {m_maxTapDelay = maxTapDelay;}
    int maxTapDelay() const
    {return m_maxTapDelay;}

    void setMaxTapDuration(int maxTapDuration)
    {m_maxTapDuration = maxTapDuration;}
    int maxTapDuration() const
    {return m_maxTapDuration;}

    void setMaxTapDistance(int maxTapDistance)
    {m_maxTapDistance = maxTapDistance;}
    int maxTapDistance() const
    {return m_maxTapDistance;}
protected:
    virtual void onTouchBegan(const Touch *touch);
    virtual void onTouchMoved(const Touch *touch);
    virtual void onTouchEnded(const Touch *touch);
private slots:
    void onTimeout();
private:
    int m_numTouchesRequired;
    int m_numTapsRequired;
    int m_maxTapDelay;
    int m_maxTapDuration;
    float m_maxTapDistance;

    bool m_numTouchesRequiredReached;
    int m_tapCounter;
    QTimer *m_timer;
    QList<Point> m_initialTouchPoints;
};

#endif /* TAPGESTURERECOGNIZER_H */
