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
#include "Loadbar.hpp"



Loadbar::Loadbar(const quint16 &speed, const Draw::Axis &axis, const QString &name, const Draw::Direction &direction, QObject *parent):
    Animation(speed,name,parent),
    m_axis(axis),
    m_direction(direction)
{
}

void Loadbar::createAnimation()
{
    fillCubeArray(0x00);
    for (quint8 i = 0; i < CUBE_SIZE; i++)
    {
        setPlane(m_axis, i);
        if(m_abort)
            return;
        waitMs(getSpeed());
    }
    waitMs(getSpeed() * 2);
    for (quint8 i = 0; i < CUBE_SIZE; i++)
    {
        clearPlane(m_axis, i);
        if(m_abort)
            return;
        waitMs(getSpeed());
    }
    Q_EMIT done();
}
