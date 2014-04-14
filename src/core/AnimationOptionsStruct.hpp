#ifndef ANIMATIONOPTIONSSTRUCT_HPP
#define ANIMATIONOPTIONSSTRUCT_HPP

#include "animations/Draw.hpp"

/*!
 \brief Animation Options Struct

*/
struct  Options{
     QString text;
     u_int16_t speed;
     u_int16_t delay;
     u_int16_t leds;
     u_int16_t iteration;
     animations::Draw::Direction direction;
     animations::Draw::Axis axis;
     bool invert;
     animations::Draw::BixelState state;
 };

#endif // ANIMATIONOPTIONSSTRUCT_HPP
