/**
 * Copyright (C) 2014  Christian Schwarzgruber <christiandev9@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef STRINGFLY_HPP
#define STRINGFLY_HPP

#include "Animation.hpp"


class StringFly : public Animation
{
    Q_OBJECT
    Q_PROPERTY(QString sToDisplay READ getSToDisplay WRITE setSToDisplay)
public:
    explicit StringFly(const u_int16_t &speed = 80,
                       const QString &string = "3D LED CUBE",
                       const QString &name = "String Fly",
                       QObject *parent = 0);

    QString getSToDisplay() const
    {
        return m_sToDisplay;
    }

public Q_SLOTS:
    virtual void createAnimation();
    void setSToDisplay(const QString &string)
    {
        if(m_sToDisplay.compare(string) != 0)
            m_sToDisplay = string;
    }

private:
    QString m_sToDisplay;
};

#endif // STRINGFLY_HPP
