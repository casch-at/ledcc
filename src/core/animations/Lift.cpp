#include "Lift.hpp"

Lift::Lift(const u_int16_t &delay, const u_int16_t  &iterations, const QString &name):
    Animation(name),
    m_delay(delay),
    m_iterations(iterations)
{
}
