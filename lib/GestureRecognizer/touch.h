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

#ifndef TOUCH_H
#define TOUCH_H

class Touch
{
public:
    Touch()
        :_touchId(0UL),_targetId(0UL),_x(0.0f),_y(0.0f),_device(nullptr)
    {}

    Touch(unsigned long touchId, unsigned long targetId,
          float x, float y, void* device)
        :_touchId(touchId),_targetId(targetId),_x(x),_y(y),_device(device)
    {}

    void setTouchId(unsigned long touchId)
    {_touchId = touchId;}
    unsigned long touchId() const
    {return _touchId;}

    void setTargetId(unsigned long targetId)
    {_targetId = targetId;}
    unsigned long targetId() const
    {return _targetId;}

    void setX(float x)
    {_x = x;}
    float x() const
    {return _x;};

    void setY(float y)
    {_y = y;}
    float y() const
    {return _y;};

    void setDevice(void* device)
    {_device = device;}
    void* device() const
    {return _device;}
private:
    unsigned long _touchId;
    unsigned long _targetId;
    float _x;
    float _y;
    void *_device;
};

#endif /* TOUCH_H */
