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

#include "target.h"
#include "gesturerecognizer.h"
#include <QtCore/QtDebug>

/**
 * @class Target
 * @brief   A target is a window with gesture recognizers attached to it.
 *
 * TargetFactory creates a target parsing a configuration file. If window
 * application class is not found in the configuration file, the target is null.
 * If it is found, target id is the window id created, and target name is
 * applicaton class name of the window. Gesture recognizers defined for this
 * target in configuration file are attached to the target.
 */

/**
 * @brief   Constructor initializes private members.
 *
 * @param[in] targetId        created window id
 * @param[in] targetName     created window application class name
 *
 * @see TargetFactory
 */
Target::Target(unsigned long targetId, const QString& targetName)
    :m_targetId(targetId), m_targetName(targetName)
{
    // We might have about 8 different gestures.
    // If we have more, no worries QList will increase its size peacefully.
    m_gestureRecognizers.reserve(8);
}

/**
 * @brief   Destructor deletes gesture recognizers attached to the target.
 *
 * When a target window is closed, its corresponding target object is deleted.
 * This destructor is called from GestureRecognizerManager when the window is
 * closed.

 * @see GestureRecognizerManager::removeTarget()
 */
Target::~Target()
{
    // When a target destroyed, its gestures also get destroyed.
    GestureRecognizer *p = nullptr;
    while (!m_gestureRecognizers.isEmpty()) {
        p = m_gestureRecognizers.takeFirst();
        p->deleteLater();
    }
}

/**
 * @brief   Attaches GestureRecognizer @p gr to this target.
 *
 * Appends @p gr to this target's gesture recognizer list.
 *
 * @param[in] gr        gesture recognizer to attach
 */
void Target::addGestureRecognizer(GestureRecognizer* gr)
{
    m_gestureRecognizers.append(gr);
}

/**
 * @brief   Helpful method for testing purposes.
 *
 * Compares this target's members with @p other target's.
 *
 * @param[in] other     target to compare with this target
 * @return              true if equal, else false.
 */
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

/**
 * @brief   Returns the list of gesture recognizer pointers.
 *
 * @return              List of gesture recognizer pointers.
 */
const QList<GestureRecognizer*>& Target::gestureRecognizers() const
{
    return m_gestureRecognizers;
}
