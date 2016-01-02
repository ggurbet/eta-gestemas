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

#ifndef STATE_H
#define STATE_H

#include <QtCore/QString>

class State
{
public:
    static const State Possible;
    static const State Failed;
    static const State Began;
    static const State Changed;
    static const State Canceled;
    static const State Ended;
    static const State Recognized;

    enum Value
    {
            Possible_,
            Failed_,
            Began_,
            Changed_,
            Canceled_,
            Ended_,
            Recognized_
    };

    explicit State(Value value = Value::Possible_)
    {m_value = value;}

    bool operator == (const State& other) const
    {return m_value == other.m_value;}

    bool operator != (const State& other) const
    {return m_value != other.m_value;}

    bool canTransitionTo(const State& newState) const;

    QString toString() const;

    bool isLeaf() const;
    bool isUnsuccessful() const;
    bool isSuccessful() const;

private:
    Value m_value;
};
#endif /* STATE_H */
