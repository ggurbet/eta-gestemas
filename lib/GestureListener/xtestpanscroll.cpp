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
#include "xtestpanscroll.h"
#include "pangesturerecognizer.h"
#include "xtest.h"
#include "utilities.h"

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
    const char* modifiers[] = {""};
    XTest::injectButton(4, modifiers);
}

void XTestPanScroll::scrollDown()
{
    const char* modifiers[] = {""};
    XTest::injectButton(5, modifiers);
}

void XTestPanScroll::scrollLeft()
{
    const char* modifiers[] = {"Control_L", ""};
    XTest::injectKey(XStringToKeysym("Left"), modifiers);
}

void XTestPanScroll::scrollRight()
{
    const char* modifiers[] = {"Control_L", ""};
    XTest::injectKey(XStringToKeysym("Right"), modifiers);
}

bool XTestPanScroll::isEqual(const GestureListener& other) const
{
    const XTestPanScroll *p = static_cast<const XTestPanScroll*>(&other);
    if (m_accumulator != p->m_accumulator) return false;
    if (m_maxVelocity != p->m_maxVelocity) return false;
    if (m_minVelocity != p->m_minVelocity) return false;

    return true;
}
