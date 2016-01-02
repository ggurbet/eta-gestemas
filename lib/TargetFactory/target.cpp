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

#include "target.h"
#include "gesturerecognizer.h"

Target::Target(unsigned long targetId, const QString& targetName)
    :m_targetId(targetId), m_targetName(targetName)
{
    // We might have about 15 different gestures.
    // If we have more, no worries QList will increase its size peacefully.
    m_gestureRecognizers.reserve(15);
}

Target::~Target()
{
    // When a target distroyed, its gestures also get destroyed.
    GestureRecognizer *p = nullptr;
    while (!m_gestureRecognizers.isEmpty()) {
        p = m_gestureRecognizers.takeFirst();
        p->deleteLater();
    }
}

void Target::addGestureRecognizer(GestureRecognizer* gr)
{
    m_gestureRecognizers.append(gr);
}

// Helpful for testing
bool Target::isEqual(const Target& other) const
{
    if (targetId() != other.targetId()) {
        return false;
    }
    if (targetName() != other.targetName()) {
        return false;
    }

    int i = 0;
    int size = other.m_gestureRecognizers.size();
    if(size != m_gestureRecognizers.size()) {
        return false;
    }
    for(i = 0; i < size; ++i) {
        if (!m_gestureRecognizers[i]->isEqual(*other.m_gestureRecognizers[i])) {
            return false;
        }
    }
    return true;
}

const QList<GestureRecognizer*>& Target::gestureRecognizers() const
{
    return m_gestureRecognizers;
}
