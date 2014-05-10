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

#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "animations/Draw.hpp"


/*!
\brief Class which holds the animation properties value

*/

class Options
{
public:
    Options(const QString &text = "", const u_int16_t &speed = 1, const u_int16_t &delay = 0,
            const u_int16_t &leds = 0, const u_int16_t &iteration = 0, const Draw::Direction &direction = Draw::Backward,
            const Draw::Axis &axis = Draw::X_AXIS, const bool invert = false, const Draw::BixelState state= Draw::OFF);
    QString m_text;
    u_int16_t m_speed;
    u_int16_t m_delay;
    u_int16_t m_leds;
    u_int16_t m_iteration;
    Draw::Direction m_direction;
    Draw::Axis m_axis;
    bool m_invert;
    Draw::BixelState m_state;
};
#endif // OPTIONS_HPP
