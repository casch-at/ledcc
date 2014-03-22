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
#include "Wall.hpp"
#include <QDebug>
Wall::Wall(const u_int16_t &speed, const Axis &axis, const Direction &direction, const QString &name, QObject *parent):
    Animation(speed,name,parent),
    m_direction(direction),
    m_axis(axis)
{
}

void Wall::createAnimation()
{
    u_int8_t i = 0;

    fillCubeArray(0x00);

    if (m_direction)
        setPlane(m_axis, 0);
    else
        setPlane(m_axis, 7);

    for (i = 0; i < CUBE_SIZE ; i++)
    {
        if(m_abort)
            return;
        waitMs(getSpeed());
        shift(m_axis, m_direction);
    }
    Q_EMIT done();
}

const QString Wall::createAnimationTooltip()
{
   QString itemToolTip;

   Animation::createAnimationTooltip(&itemToolTip);

   if(m_axis == X_AXIS)
       itemToolTip.append("Axis: X-Axis<br>");
   else if(m_axis == Y_AXIS)
       itemToolTip.append("Axis: Y-Axis<br>");
   else
       itemToolTip.append("Axis: Z-Axis<br>");

   itemToolTip.append(m_direction == Draw::FORWARD ? "Direction: Forward<br>" : "Direction: Backward<br>");

   return itemToolTip;
}
