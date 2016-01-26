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

#include "xtestscroll.h"
#include "pangesturerecognizer.h"
#include "xtestshortcut.h"
#include "utilities.h"
#include <QtCore/QtDebug>

XTestScroll::XTestScroll()
    :m_minEffectiveVelocity(MIN_EFFECTIVE_VELOCITY),
     m_recognizer(nullptr),
     m_upShortcut(nullptr),
     m_downShortcut(nullptr),
     m_leftShortcut(nullptr),
     m_rightShortcut(nullptr)
{
}

XTestScroll::~XTestScroll()
{
    delete m_upShortcut;
    delete m_downShortcut;
    delete m_leftShortcut;
    delete m_rightShortcut;
    m_upShortcut = nullptr;
    m_downShortcut = nullptr;
    m_leftShortcut = nullptr;
    m_rightShortcut = nullptr;
}

void XTestScroll::setGestureRecognizer(const PanGestureRecognizer *recognizer)
{
    m_recognizer = recognizer;
}

void XTestScroll::setScrollUpShortcut(const XTestShortcut *upShortcut)
{
    m_upShortcut = upShortcut;
}

void XTestScroll::setScrollDownShortcut(const XTestShortcut *downShortcut)
{
    m_downShortcut = downShortcut;
}

void XTestScroll::setScrollLeftShortcut(const XTestShortcut *leftShortcut)
{
    m_leftShortcut = leftShortcut;
}

void XTestScroll::setScrollRightShortcut(const XTestShortcut *rightShortcut)
{
    m_rightShortcut = rightShortcut;
}

void XTestScroll::onBegan()
{
}
void XTestScroll::onRecognized()
{
}

void XTestScroll::onChanged()
{
    float vx = m_recognizer->velocityX();
    float vy = m_recognizer->velocityY();
    float absvx = fabsf(vx);
    float absvy = fabsf(vy);

    if (absvx > absvy) {
        if (vx >= m_minEffectiveVelocity) {
            scrollLeft();
        } else if (vx < -m_minEffectiveVelocity) {
            scrollRight();
        }
    } else if (absvx < absvy) {
        if (vy >= m_minEffectiveVelocity) {
            scrollUp();
        } else if (vy < -m_minEffectiveVelocity) {
            scrollDown();
        }
    }
}

void XTestScroll::onCanceled()
{
}

void XTestScroll::onEnded()
{
}

void XTestScroll::onFailed()
{
}

void XTestScroll::scrollUp()
{
    m_upShortcut->press();
    m_upShortcut->release();
}

void XTestScroll::scrollDown()
{
    m_downShortcut->press();
    m_downShortcut->release();
}

void XTestScroll::scrollLeft()
{
    m_leftShortcut->press();
    m_leftShortcut->release();
}

void XTestScroll::scrollRight()
{
    m_rightShortcut->press();
    m_rightShortcut->release();
}

bool XTestScroll::isEqual(const GestureListener& other) const
{
    const XTestScroll *p = static_cast<const XTestScroll *>(&other);
    if (m_minEffectiveVelocity != p->m_minEffectiveVelocity) return false;

    return true;
}
