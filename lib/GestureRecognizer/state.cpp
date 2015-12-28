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

#include "state.h"

#define DECLARE_STATE(state) const State State::state(state##_);
DECLARE_STATE(Possible)
DECLARE_STATE(Failed)
DECLARE_STATE(Began)
DECLARE_STATE(Changed)
DECLARE_STATE(Canceled)
DECLARE_STATE(Ended)
DECLARE_STATE(Recognized)

bool State::canTransitionTo(const State& newState) const
{
    switch (m_value) {
    case Possible_:
        switch (newState.m_value) {
        case Recognized_:
        case Failed_:
        case Began_:
            return true;
        default:
            return false;
        }
    case Began_:
    case Changed_:
        switch (newState.m_value) {
        case Changed_:
        case Ended_:
        case Canceled_:
            return true;
        default:
            return false;
        }
    case Recognized_:
    case Ended_:
    case Failed_:
    case Canceled_:
        switch (newState.m_value) {
        case Possible_:
            return true;
        default:
            return false;
        }
    default:
        return false;
    }
}
