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

#ifndef XTESTTWOTOUCHPINCHZOOM_H
#define XTESTTWOTOUCHPINCHZOOM_H

#include "twotouchpinchlistener.h"

class XTestTwoTouchPinchZoom : public TwoTouchPinchListener
{
public:
    XTestTwoTouchPinchZoom() = default;
    virtual ~XTestTwoTouchPinchZoom() = default;
    virtual bool isEqual(const GestureListener& other) const;
    virtual void onBegan();
    virtual void onRecognized();
    virtual void onChanged();
    virtual void onCanceled();
    virtual void onEnded();
    virtual void onFailed();

    void setMaxScale(float maxScale)
    {m_maxScale = maxScale;}
    float maxScale() const
    {return m_maxScale;}

    void setMinScale(float minScale)
    {m_minScale = minScale;}
    float minScale() const
    {return m_minScale;}

    void setAccumulator(int accumulator)
    {m_accumulator = accumulator;}
    int accumulator() const
    {return m_accumulator;}
private:
    void zoomIn();
    void zoomOut();

    float m_maxScale;
    float m_minScale;
    int m_accumulator;
    int m_counter;
    float m_averageScale;
};

#endif /* XTESTTWOTOUCHPINCHZOOM_H */
