#ifndef LIFT_HPP
#define LIFT_HPP
#include "Animation.hpp"


class Lift : public Animation
{
public:
    Lift(const u_int16_t &delay = 50, const u_int16_t  &iterations = 10, const QString &name = "Lift");
    void setDelay(const u_int16_t &d){
        if(m_delay != d){
            m_delay = d;
        }
    }
    u_int16_t getDelay(void) const{
        return m_delay;
    }

    void setIterations(const u_int16_t &i){
        if (m_iterations != i) {
            m_iterations = i;
        }
    }
private:
    u_int16_t m_delay;
    u_int16_t m_iterations;
};

#endif // LIFT_HPP
