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
#include "Rain.hpp"



Rain::Rain(const quint16 &iterations, const quint16 &speed, const QString &name, QObject *parent):
    Animation(speed,name,parent),
    m_iterations(iterations)
{
}

void Rain::createAnimation()
{
    quint8 i = 0;
    quint16 ii = 0;

    for (ii = 0; ii < m_iterations; ii++)
    {
        for (i = 0; i < qrand() % (CUBE_SIZE / 2); i++)
        {
            setBixel(qrand() % CUBE_SIZE, qrand() % CUBE_SIZE, 7);
        }
        if(m_abort)
            return;
        waitMs(getSpeed());
        shift(Z_AXIS, Backward);
    }
    Q_EMIT done();
}
