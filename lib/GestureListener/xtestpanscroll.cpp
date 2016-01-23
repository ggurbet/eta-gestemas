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

#include "xtestpanscroll.h"
#include "pangesturerecognizer.h"
#include "xtestshortcut.h"
#include "utilities.h"
#include <QtCore/QtDebug>

XTestPanScroll::XTestPanScroll()
    :m_upShortcut(nullptr),
     m_downShortcut(nullptr),
     m_leftShortcut(nullptr),
     m_rightShortcut(nullptr)
{
}

XTestPanScroll::~XTestPanScroll()
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

void XTestPanScroll::setScrollUpShortcut(const XTestShortcut *upShortcut)
{
    m_upShortcut = upShortcut;
}

void XTestPanScroll::setScrollDownShortcut(const XTestShortcut *downShortcut)
{
    m_downShortcut = downShortcut;
}

void XTestPanScroll::setScrollLeftShortcut(const XTestShortcut *leftShortcut)
{
    m_leftShortcut = leftShortcut;
}

void XTestPanScroll::setScrollRightShortcut(const XTestShortcut *rightShortcut)
{
    m_rightShortcut = rightShortcut;
}

void XTestPanScroll::onBegan()
{
    m_counter = m_accumulator;
    m_averageVelocityX = 0.0f;
    m_averageVelocityY = 0.0f;
}
void XTestPanScroll::onRecognized()
{
}
void XTestPanScroll::onChanged()
{
    const PanGestureRecognizer *p = m_recognizer;
    if (m_counter != 0) {
        m_averageVelocityX += p->velocityX();
        m_averageVelocityY += p->velocityY();
        --m_counter;
        return;
    }
    m_averageVelocityX /= m_accumulator;
    m_averageVelocityY /= m_accumulator;

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
void XTestPanScroll::onCanceled()
{
}

void XTestPanScroll::onEnded()
{
}

void XTestPanScroll::onFailed()
{
}

void XTestPanScroll::scrollUp()
{
    m_upShortcut->press();
    m_upShortcut->release();
}

void XTestPanScroll::scrollDown()
{
    m_downShortcut->press();
    m_downShortcut->release();
}

void XTestPanScroll::scrollLeft()
{
    m_leftShortcut->press();
    m_leftShortcut->release();
}

void XTestPanScroll::scrollRight()
{
    m_rightShortcut->press();
    m_rightShortcut->release();
}

bool XTestPanScroll::isEqual(const GestureListener& other) const
{
    const XTestPanScroll *p = static_cast<const XTestPanScroll*>(&other);
    if (m_accumulator != p->m_accumulator) return false;
    if (m_maxVelocity != p->m_maxVelocity) return false;
    if (m_minVelocity != p->m_minVelocity) return false;

    return true;
}
