#ifndef LIFT_HPP
#define LIFT_HPP
#include "Animation.hpp"


class Lift : public Animation
{
    Q_OBJECT
    Q_PROPERTY(u_int16_t iterations READ getIterations WRITE setIterations)
    Q_PROPERTY(u_int16_t delay READ getDelay WRITE setDelay)
public:
   explicit Lift(const u_int16_t &delay = 50, const u_int16_t  &iterations = 10,
         const QString &name = "Lift", const u_int16_t &speed = 80, QObject *parent = Q_NULLPTR);
    virtual void createAnimation();
    u_int16_t getIterations() const
    {
        return m_iterations;
    }

    u_int16_t getDelay() const
    {
        return m_delay;
    }

public slots:
    void setIterations(const u_int16_t &arg)
    {
        if(m_iterations != arg)
            m_iterations = arg;
    }

    void setDelay(const u_int16_t &arg)
    {
        if(m_delay != arg)
            m_delay = arg;
    }

private:
    u_int16_t m_delay;
    u_int16_t m_iterations;
};

#endif // LIFT_HPP
