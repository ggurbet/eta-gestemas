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

    XTestShortcut() : m_type(Undefined) {}
    ~XTestShortcut() = default;
    void setValue(const QString& value, Type type);
    void addModifier(const QString& modifier);
    void press() const;
    void release() const;
    void movePointer(float x, float y) const;
private:
    QStringList m_modifiers;
    Type m_type;
    QString m_keyValue;
    int m_buttonValue;
};

#endif /* XTESTSHORTCUT_H */
