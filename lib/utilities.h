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

#ifndef UTILITIES_H
#define UTILITIES_H

#define SQUARED(x) ((x) *(x))

#define SQUARED_PYTHAGOREAN(y1, y2, x1, x2) \
    (SQUARED((x2) - (x1)) + SQUARED((y2) - (y1)))

#define CHECK_RANGE(val, min, max)          \
    ((val) >= (min) && (val) <= (max))

#endif /* UTILITIES_H */
