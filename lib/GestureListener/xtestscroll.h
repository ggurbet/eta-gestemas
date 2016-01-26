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

#ifndef SCROLL_H
#define SCROLL_H

#include "gesturelistener.h"

class XTestShortcut;
class PanGestureRecognizer;

class XTestScroll : public GestureListener
{
public:
    XTestScroll();
    virtual ~XTestScroll();
    virtual bool isEqual(const GestureListener& other) const;
    virtual void onBegan();
    virtual void onRecognized();
    virtual void onChanged();
    virtual void onCanceled();
    virtual void onEnded();
    virtual void onFailed();

    void setMinEffectiveVelocity(float velocity)
    {m_minEffectiveVelocity = (velocity > 0.0f) ? velocity : MIN_EFFECTIVE_VELOCITY;}

    void setGestureRecognizer(const PanGestureRecognizer *recognizer);

    void setScrollUpShortcut(const XTestShortcut *upShortcut);
    void setScrollDownShortcut(const XTestShortcut *downShortcut);
    void setScrollLeftShortcut(const XTestShortcut *leftShortcut);
    void setScrollRightShortcut(const XTestShortcut *rightShortcut);
private:
    void scrollUp();
    void scrollDown();
    void scrollLeft();
    void scrollRight();

    float m_minEffectiveVelocity;
    const PanGestureRecognizer *m_recognizer;
    const XTestShortcut *m_upShortcut;
    const XTestShortcut *m_downShortcut;
    const XTestShortcut *m_leftShortcut;
    const XTestShortcut *m_rightShortcut;

    static constexpr float MIN_EFFECTIVE_VELOCITY = 2.0f;
};

#endif /* SCROLL_H */
