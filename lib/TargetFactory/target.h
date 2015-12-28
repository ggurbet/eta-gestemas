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

#ifndef TARGET_H
#define TARGET_H

#include <QtCore/QString>
#include <QtCore/QList>

class GestureRecognizer;

class Target
{
public:
    explicit Target(unsigned long targetId = 0, const QString& targetName=QString());
    ~Target();
    void addGestureRecognizer(GestureRecognizer* gr);

    void setTargetId(unsigned long targetId)
    {m_targetId = targetId;}
    unsigned long targetId() const
    {return m_targetId;}

    void setTargetName(const QString& targetName)
    {m_targetName = targetName;}
    const QString& targetName() const
    {return m_targetName;}

    bool isEqual(const Target& other) const;

    Target(const Target&) = delete;
    Target& operator=(const Target&) = delete;
private:
    unsigned long m_targetId;
    QString m_targetName;
    QList<GestureRecognizer*> m_gestureRecognizers;
};

#endif /* TARGET_H */
