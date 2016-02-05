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

#ifndef XTESTSHORTCUT_H
#define XTESTSHORTCUT_H

#include <QtCore/QStringList>
#include <QtCore/QString>

class XTestShortcut
{
public:
    enum Type
    {
        Undefined,
        Button,
        Key
    };

    enum Action
    {
        NoAction,
        Press,
        Release,
        PressRelease
    };

    XTestShortcut()
        : m_type(Undefined),
          m_onBeganAction(NoAction),
          m_onRecognizedAction(NoAction),
          m_onChangedAction(NoAction),
          m_onCanceledAction(NoAction),
          m_onEndedAction(NoAction),
          m_onFailedAction(NoAction) {}
    ~XTestShortcut() = default;
    void setValue(const QString& value, Type type);
    void addModifier(const QString& modifier);
    void press() const;
    void release() const;
    void pressAndRelease() const;
    void movePointer(float x, float y) const;

    void setOnBeganAction(Action a)
    {m_onBeganAction = a;}
    Action onBeganAction() const
    {return m_onBeganAction;}

    void setOnRecognizedAction(Action a)
    {m_onRecognizedAction = a;}
    Action onRecognizedAction() const
    {return m_onRecognizedAction;}

    void setOnChangedAction(Action a)
    {m_onChangedAction = a;}
    Action onChangedAction() const
    {return m_onChangedAction;}

    void setOnCanceledAction(Action a)
    {m_onCanceledAction = a;}
    Action onCanceledAction() const
    {return m_onCanceledAction;}

    void setOnEndedAction(Action a)
    {m_onEndedAction = a;}
    Action onEndedAction() const
    {return m_onEndedAction;}

    void setOnFailedAction(Action a)
    {m_onFailedAction = a;}
    Action onFailedAction() const
    {return m_onFailedAction;}
private:
    QStringList m_modifiers;
    Type m_type;
    QString m_keyValue;
    int m_buttonValue;
    Action m_onBeganAction;
    Action m_onRecognizedAction;
    Action m_onChangedAction;
    Action m_onCanceledAction;
    Action m_onEndedAction;
    Action m_onFailedAction;
};

#endif /* XTESTSHORTCUT_H */
