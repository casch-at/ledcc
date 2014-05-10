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

#include "Options.hpp"


Options::Options(const QString &text, const u_int16_t &speed, const u_int16_t &delay,
                 const u_int16_t &leds, const u_int16_t &iteration,  const Draw::Direction &direction,
                 const Draw::Axis &axis, const bool invert, const Draw::BixelState state):
    m_text(text),
    m_speed(speed),
    m_delay(delay),
    m_leds(leds),
    m_iteration(iteration),
    m_direction(direction),
    m_axis(axis),
    m_invert(invert),
    m_state(state)
{

}
