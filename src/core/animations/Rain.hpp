#ifndef RAIN_HPP
#define RAIN_HPP
#include "Animation.hpp"

class Rain : public Animation
{
    Q_OBJECT
    Q_PROPERTY(u_int16_t iterations READ getIterations WRITE setIterations)
public:
    explicit Rain(const u_int16_t &iterations = 20, const u_int16_t &speed = 80,
                  const QString &name = "Rain",QObject *parent = Q_NULLPTR);

    virtual void createAnimation();
    u_int16_t getIterations() const
    {
        return m_iterations;
    }

public slots:
    void setIterations(const u_int16_t &arg)
    {
        if(m_iterations != arg)
            m_iterations = arg;
    }

private:
    u_int16_t m_iterations;
};

#endif // RAIN_HPP
