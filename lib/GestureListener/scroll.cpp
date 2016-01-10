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
#include "scroll.h"
#include "pangesturerecognizer.h"
#include "utilities.h"

void Scroll::setGestureRecognizer(PanGestureRecognizer *recognizer)
{
    GestureListener::setGestureRecognizer(recognizer);
}

void Scroll::onBegan()
{
    qDebug() << "Scroll onBegan";
    m_counter = m_accumulator;
    m_averageVelocityX = 0.0f;
    m_averageVelocityY = 0.0f;
}
void Scroll::onRecognized()
{
    qDebug() << "Scroll onRecognized";
}
void Scroll::onChanged()
{
    const PanGestureRecognizer *p =
        static_cast<const PanGestureRecognizer*>(m_recognizer);
    if (m_counter != 0) {
        m_averageVelocityX += p->velocityX();
        m_averageVelocityY += p->velocityY();
        --m_counter;
        return;
    }
    qDebug() << "Scroll onChanged";
    m_averageVelocityX /= m_accumulator;
    m_averageVelocityY /= m_accumulator;

    qDebug() << "vx: " << m_averageVelocityX
             << " vy: " << m_averageVelocityY;
    if (m_averageVelocityX > 0.0f) {
        if (CHECK_RANGE(m_averageVelocityX, minVelocity(), maxVelocity())) {
            scrollLeft();
        }
    } else {
        if (CHECK_RANGE(m_averageVelocityX, -maxVelocity(), -minVelocity())) {
            scrollRight();
        }
    }
    if (m_averageVelocityY > 0.0f) {
        if (CHECK_RANGE(m_averageVelocityY, minVelocity(), maxVelocity())) {
            scrollUp();
        }
    } else {
        if (CHECK_RANGE(m_averageVelocityY, -maxVelocity(), -minVelocity())) {
            scrollDown();
        }
    }
    m_averageVelocityX = 0.0f;
    m_averageVelocityY = 0.0f;
    m_counter = m_accumulator;
}
void Scroll::onCanceled()
{
    qDebug() << "Scroll onCanceled";
}
void Scroll::onEnded()
{
    qDebug() << "Scroll onEnded";
}
void Scroll::onFailed()
{
    qDebug() << "Scroll onFailed";
}

void Scroll::scrollUp()
{

    const char* modifiers[] = {""};
    injectButton(5, modifiers);
}

void Scroll::scrollDown()
{
    const char* modifiers[] = {""};
    injectButton(4, modifiers);
}

void Scroll::scrollLeft()
{
    const char* modifiers[] = {"Control_L", ""};
    injectKey(XStringToKeysym("Left"), modifiers);
}

void Scroll::scrollRight()
{
    const char* modifiers[] = {"Control_L", ""};
    injectKey(XStringToKeysym("Right"), modifiers);
}

bool Scroll::isEqual(const GestureListener& other) const
{
    const Scroll *p = static_cast<const Scroll*>(&other);
    if (m_accumulator != p->m_accumulator) return false;
    if (m_maxVelocity != p->m_maxVelocity) return false;
    if (m_minVelocity != p->m_minVelocity) return false;

    return true;
}
