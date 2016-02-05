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

#include "xtestsimple.h"
#include "gesturerecognizer.h"
#include "xtestshortcut.h"
#include "utilities.h"
#include <QtCore/QtDebug>

XTestSimple::XTestSimple()
    :m_shortcut(nullptr)
{
}

XTestSimple::~XTestSimple()
{
    delete m_shortcut;
    m_shortcut = nullptr;
}

void XTestSimple::setShortcut(const XTestShortcut *shortcut)
{
    m_shortcut = shortcut;
}

void XTestSimple::onBegan()
{
    if (m_shortcut) {
        switch (m_shortcut->onBeganAction()) {
        case XTestShortcut::Press:
            m_shortcut->press();
            break;
        case XTestShortcut::Release:
            m_shortcut->release();
            break;
        case XTestShortcut::PressRelease:
            m_shortcut->pressAndRelease();
            break;
        default:
            break;
        }
    }
}

void XTestSimple::onRecognized()
{
    if (m_shortcut) {
        switch (m_shortcut->onRecognizedAction()) {
        case XTestShortcut::Press:
            m_shortcut->press();
            break;
        case XTestShortcut::Release:
            m_shortcut->release();
            break;
        case XTestShortcut::PressRelease:
            m_shortcut->pressAndRelease();
            break;
        default:
            break;
        }
    }
}

void XTestSimple::onChanged()
{
    if (m_shortcut) {
        switch (m_shortcut->onChangedAction()) {
        case XTestShortcut::Press:
            m_shortcut->press();
            break;
        case XTestShortcut::Release:
            m_shortcut->release();
            break;
        case XTestShortcut::PressRelease:
            m_shortcut->pressAndRelease();
            break;
        default:
            break;
        }
    }
}

void XTestSimple::onCanceled()
{
    if (m_shortcut) {
        switch (m_shortcut->onCanceledAction()) {
        case XTestShortcut::Press:
            m_shortcut->press();
            break;
        case XTestShortcut::Release:
            m_shortcut->release();
            break;
        case XTestShortcut::PressRelease:
            m_shortcut->pressAndRelease();
            break;
        default:
            break;
        }
    }
}

void XTestSimple::onEnded()
{
    if (m_shortcut) {
        switch (m_shortcut->onEndedAction()) {
        case XTestShortcut::Press:
            m_shortcut->press();
            break;
        case XTestShortcut::Release:
            m_shortcut->release();
            break;
        case XTestShortcut::PressRelease:
            m_shortcut->pressAndRelease();
            break;
        default:
            break;
        }
    }
}

void XTestSimple::onFailed()
{
    if (m_shortcut) {
        switch (m_shortcut->onFailedAction()) {
        case XTestShortcut::Press:
            m_shortcut->press();
            break;
        case XTestShortcut::Release:
            m_shortcut->release();
            break;
        case XTestShortcut::PressRelease:
            m_shortcut->pressAndRelease();
            break;
        default:
            break;
        }
    }
}

bool XTestSimple::isEqual(const GestureListener& other) const
{
    (void)other;
    return true;
}
