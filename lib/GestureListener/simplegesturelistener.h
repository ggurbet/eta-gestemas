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

#ifndef SIMPLEGESTURELISTENER_H
#define SIMPLEGESTURELISTENER_H

#include "gesturelistener.h"

class GestureRecognizer;

class SimpleGestureListener : public GestureListener
{
public:
    SimpleGestureListener() = default;
    virtual ~SimpleGestureListener() = default;
    void setGestureRecognizer(const GestureRecognizer *recognizer);
protected:
    const GestureRecognizer *m_recognizer;
};


#endif /* SIMPLEGESTURELISTENER_H */
