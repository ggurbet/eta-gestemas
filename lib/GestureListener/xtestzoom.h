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

#ifndef XTESTZOOM_H
#define XTESTZOOM_H

#include "gesturelistener.h"

class XTestShortcut;
class PinchGestureRecognizer;

class XTestZoom : public GestureListener
{
public:
    XTestZoom();
    virtual ~XTestZoom();
    virtual bool isEqual(const GestureListener& other) const;
    virtual void onBegan();
    virtual void onRecognized();
    virtual void onChanged();
    virtual void onCanceled();
    virtual void onEnded();
    virtual void onFailed();

    void setMinEffectiveVelocity(float velocity)
    {m_minEffectiveVelocity = (velocity > 0.0f) ? velocity : MIN_EFFECTIVE_VELOCITY;}

    void setGestureRecognizer(const PinchGestureRecognizer *recognizer);

    void setZoomInShortcut(const XTestShortcut *zoomInShortcut);
    void setZoomOutShortcut(const XTestShortcut *zoomOutShortcut);
private:
    void zoomIn();
    void zoomOut();

    float m_minEffectiveVelocity; // (oldScale - newScale / second)
    const PinchGestureRecognizer *m_recognizer;
    const XTestShortcut *m_zoomInShortcut;
    const XTestShortcut *m_zoomOutShortcut;

    static constexpr float MIN_EFFECTIVE_VELOCITY = 0.5f;
};

#endif /* XTESTZOOM_H */
