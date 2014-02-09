#ifndef LIFT_HPP
#define LIFT_HPP
#include "Animation.hpp"


class Lift : public Animation
{
public:
   explicit Lift(const u_int16_t &delay = 50, const u_int16_t  &iterations = 10,
         const QString &name = "Lift", const u_int16_t &speed = 80, QObject *parent = Q_NULLPTR):
        Animation(speed,name,parent),
        m_delay(delay),
        m_iterations(iterations){
    }
    void setDelay(const u_int16_t &delay){
        if(m_delay != delay){
            m_delay = delay;
        }
    }
    u_int16_t getDelay(void) const{
        return m_delay;
    }

    void setIterations(const u_int16_t &iterations){
        if (m_iterations != iterations) {
            m_iterations = iterations;
        }
    }
    u_int16_t getIterations(void) const{
       return m_iterations;
    }
    virtual void createAnimation();
private:
    u_int16_t m_delay;
    u_int16_t m_iterations;
};

#endif // LIFT_HPP
