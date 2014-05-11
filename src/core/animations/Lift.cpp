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
#include "Lift.hpp"



Lift::Lift(const quint16 &delay, const quint16 &iterations, const QString &name, const quint16 &speed, QObject *parent):
    Animation(speed,name,parent),
    m_delay(delay),
    m_iterations(iterations)
{
}

void Lift::createAnimation()
{

    quint8 lastX = 0;
    quint8 lastY = 0;
    quint8 x;
    quint8 y;

    fillCubeArray(0x00);

    for (x = 0; x < CUBE_SIZE; x++)
    {
        for (y = 0; y < CUBE_SIZE; y++)
        {
            setBixel(x, y, (qrand() % 2) * 7);
        }
    }

    for (quint16 i = 0; i < m_iterations; i++)
    {

        x = qrand() % CUBE_SIZE;
        y = qrand() % CUBE_SIZE;

        if (y != lastY && x != lastX)
        {
            if (getBixelState(x, y, 0))
                sendBixelZ(x, y, 0);
            else
                sendBixelZ(x, y, 7);
            if(m_abort)
                return;
            waitMs(m_delay);
            lastX = x;
            lastY = y;
        }
    }
    Q_EMIT done();
}

