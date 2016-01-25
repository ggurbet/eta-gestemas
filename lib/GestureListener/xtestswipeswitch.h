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

#ifndef XTESTSWIPESWITCH_H
#define XTESTSWIPESWITCH_H

#include "swipelistener.h"

class XTestShortcut;

class XTestSwipeSwitch : public SwipeListener
{
public:
    XTestSwipeSwitch();
    virtual ~XTestSwipeSwitch();
    virtual bool isEqual(const GestureListener& other) const;
    virtual void onBegan();
    virtual void onRecognized();
    virtual void onChanged();
    virtual void onCanceled();
    virtual void onEnded();
    virtual void onFailed();
    void setSwitchRightShortcut(const XTestShortcut *shortcut);
    void setSwitchLeftShortcut(const XTestShortcut *shortcut);
private:
    const XTestShortcut *m_switchRightShortcut;
    const XTestShortcut *m_switchLeftShortcut;
};

#endif /* XTESTSWIPESWITCH_H */
