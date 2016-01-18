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

#include <cstdint>

class Touch
{
public:
    enum OwnershipState {
        Deferred,
        Accepted,
        Rejected
    };

    Touch();
    Touch(uint32_t touchId, uint32_t targetId,
          float startX, float startY,
          float resolutionX, float resolutionY,
          void *device, uint64_t timestamp);

    ~Touch() = default;
    Touch(const Touch&) = default;
    Touch& operator=(const Touch&) = default;

    void setTouchId(uint32_t touchId)
    {m_touchId = touchId;}
    uint32_t touchId() const
    {return m_touchId;}

    void setTargetId(uint32_t targetId)
    {m_targetId = targetId;}
    uint32_t targetId() const
    {return m_targetId;}

    void setStartX(float x)
    {m_startX = x;}
    float startX() const
    {return m_startX;}
    float startXInMeter() const
    {return m_startX / m_resolutionX;}

    void setStartY(float y)
    {m_startY = y;}
    float startY() const
    {return m_startY;}
    float startYInMeter() const
    {return m_startY / m_resolutionY;}

    void setPreviousX(float x)
    {m_previousX = x;}
    float previousX() const
    {return m_previousX;}
    float previousXInMeters() const
    {return m_previousX / m_resolutionX;}

    void setPreviousY(float y)
    {m_previousY = y;}
    float previousY() const
    {return m_previousY;}
    float previousYInMeters() const
    {return m_previousY / m_resolutionY;}

    void computeX(float deltaX)
    {m_cumulativeDeltaX += deltaX;}
    float computedX() const
    {return m_startX + m_cumulativeDeltaX;}
    float computedXInMeters() const
    {return computedX() / m_resolutionX;}

    void computeY(float deltaY)
    {m_cumulativeDeltaY += deltaY;}
    float computedY() const
    {return m_startY + m_cumulativeDeltaY;}
    float computedYInMeters() const
    {return computedY() / m_resolutionY;}

    float cumulativeDeltaX() const
    {return m_cumulativeDeltaX;}

    float cumulativeDeltaY() const
    {return m_cumulativeDeltaY;}

    float cumulativeDeltaXInMeters() const
    {return m_cumulativeDeltaX / m_resolutionX;}

    float cumulativeDeltaYInMeters() const
    {return m_cumulativeDeltaY / m_resolutionY;}

    void setResolutionX(float x)
    {m_resolutionX = x;}
    float resolutionX() const
    {return m_resolutionX;}

    void setResolutionY(float y)
    {m_resolutionY = y;}
    float resolutionY() const
    {return m_resolutionX;}

    void setDevice(void* device)
    {m_device = device;}
    void* device() const
    {return m_device;}

    void setStartTime(uint64_t startTime)
    {m_startTime = startTime;}
    uint64_t startTime() const
    {return m_startTime;}

    void setTimestamp(uint64_t timestamp)
    {m_timestamp = timestamp;}
    uint64_t timestamp() const
    {return m_timestamp;}

    void setOwnershipState(OwnershipState ownershipState);
    OwnershipState ownershipState() const
    {return m_ownershipState;}
private:
    uint32_t m_touchId;
    uint32_t m_targetId;
    float m_startX;
    float m_startY;
    float m_previousX;
    float m_previousY;
    float m_cumulativeDeltaX;
    float m_cumulativeDeltaY;
    float m_resolutionX;
    float m_resolutionY;
    void *m_device;
    uint64_t m_startTime;
    uint64_t m_timestamp;
    OwnershipState m_ownershipState;
};

#endif /* TOUCH_H */
