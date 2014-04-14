#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "animations/Draw.hpp"


/*!
\brief Class which holds the animation properties value

*/
namespace animations {
    class Options
    {
    public:
        Options(const QString &text = "", const u_int16_t &speed = 1, const u_int16_t &delay = 0,
                const u_int16_t &leds = 1, const u_int16_t &iteration = 0,
                const Draw::Direction &direction = Draw::Backward, const Draw::Axis &axis = Draw::X_AXIS);
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
    } // End namespace animations
#endif // OPTIONS_HPP
