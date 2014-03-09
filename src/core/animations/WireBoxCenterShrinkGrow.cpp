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
#include "WireBoxCenterShrinkGrow.hpp"

WireBoxCenterShrinkGrow::WireBoxCenterShrinkGrow(const u_int16_t &speed, const bool centerStart, const u_int16_t &iterations, const QString &name, QObject *parent) :
    Animation(speed,name,parent),
    m_centerStart(centerStart),
    m_iterations(iterations)
{
}

void WireBoxCenterShrinkGrow::createAnimation()
{
    u_int8_t j = 0;
    u_int8_t cubeSizeHalf = CUBE_SIZE / 2;
    u_int8_t xyzStart = 0;
    u_int8_t xyzEnd = 0;
    for(u_int8_t iter = 0; iter < m_iterations; iter++){
        for (u_int8_t i = 0; i < cubeSizeHalf; i++)
        {
            fillCubeArray(0x00);
            j = i;
            if (m_centerStart)
                j = cubeSizeHalf -1 - i;
            xyzStart = cubeSizeHalf + j;
            xyzEnd = cubeSizeHalf - 1 - j;
            boxWireframe(xyzStart, xyzStart, xyzStart, xyzEnd, xyzEnd, xyzEnd);
            if(m_abort)
                return;
            waitMs(getSpeed());
        }
    }
    Q_EMIT done();
}


