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

#include <QtCore/QtDebug>
#include "leftclick.h"
#include "tapgesturerecognizer.h"

void LeftClick::setGestureRecognizer(TapGestureRecognizer *recognizer)
{
    GestureListener::setGestureRecognizer(recognizer);
}

void LeftClick::onBegan()
{
    qDebug() << "LeftClick onBegan";
}
void LeftClick::onRecognized()
{
    qDebug() << "LeftClick onRecognized";
    const char *modifiers[] = {""};
    injectButton(1, modifiers);
}
void LeftClick::onChanged()
{
    qDebug() << "LeftClick onChanged";
}
void LeftClick::onCanceled()
{
    qDebug() << "LeftClick onCanceled";
}
void LeftClick::onEnded()
{
    qDebug() << "LeftClick onEnded";
}
void LeftClick::onFailed()
{
    qDebug() << "LeftClick onFailed";
}

bool LeftClick::isEqual(const GestureListener& other) const
{
    (void)other;
    return true;
}
