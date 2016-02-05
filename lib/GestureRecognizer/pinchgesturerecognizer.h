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

#ifndef PINCHGESTURERECOGNIZER_H
#define PINCHGESTURERECOGNIZER_H

#include "gesturerecognizer.h"

class Touch;

class PinchGestureRecognizer : public GestureRecognizer
{
public:
    PinchGestureRecognizer() = default;
    virtual ~PinchGestureRecognizer() = default;
    virtual float scale() const = 0;
    virtual float velocity() const = 0;
};

#endif /* PINCHGESTURERECOGNIZER_H */
