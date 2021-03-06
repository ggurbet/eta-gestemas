/* Copyright (C) 2015-2016 Gökhan Karabulut <gokhan.karabulut@tubitak.gov.tr>
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

#ifndef LONGPRESSGESTURERECOGNIZER_H
#define LONGPRESSGESTURERECOGNIZER_H

#include "gesturerecognizer.h"

class QTimer;

class LongPressGestureRecognizer : public GestureRecognizer
{
public:
    LongPressGestureRecognizer();
    virtual bool isEqual(const GestureRecognizer& other) const;
    virtual void reset();

    void setNumTouchesRequired(int numTouchesRequired)
    {m_numTouchesRequired = numTouchesRequired;}
    int numTouchesRequired() const
    {return m_numTouchesRequired;}

    void setMinPressDuration(int msec)
    {m_minPressDuration = msec;}
    int minPressDuration() const
    {return m_minPressDuration;}
protected:
    virtual void onTouchBegan(const Touch *touch);
    virtual void onTouchMoved(const Touch *touch);
    virtual void onTouchEnded(const Touch *touch);
private slots:
    void onTimeout();

private:
    int m_numTouchesRequired;
    int m_minPressDuration; //ms

    bool m_numTouchesRequiredReached;
    QTimer *m_timer;
};

#endif /* LONGPRESSGESTURERECOGNIZER_H */
