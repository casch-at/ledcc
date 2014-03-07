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
#include "RandomFiller.hpp"

RandomFiller::RandomFiller(const u_int16_t &speed, const BixelState &state, const QString &name, QObject *parent) :
    Animation(speed,name,parent),
    m_state(state)
{
}

void RandomFiller::createAnimation()
{
    u_int32_t leds = 0;
    u_int8_t x = 0;
    u_int8_t y = 0;
    u_int8_t z = 0;

    if (m_state)
        fillCubeArray(0x00);
    else
        fillCubeArray(0xFF);

    while (leds < CUBE_LED_COUNT - 1)
    {
        x = qrand() % CUBE_SIZE;
        y = qrand() % CUBE_SIZE;
        z = qrand() % CUBE_SIZE;
        if ((m_state == OFF && getBixelState(x, y, z) == ON)
                || (m_state == ON && getBixelState(x, y, z) == OFF))
        {
            alterBixel(x, y, z, m_state);
            if(m_abort)
                return;
            waitMs(getSpeed());
            leds++;
        }
    }
    Q_EMIT done();
}
