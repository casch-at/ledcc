#ifndef RANDOMZLIFT_HPP
#define RANDOMZLIFT_HPP

#include "Animation.hpp"

class RandomZLift : public Animation
{
    Q_OBJECT
    Q_PROPERTY(u_int16_t iterations READ getIterations WRITE setIterations)
public:
    explicit RandomZLift(const u_int16_t &iterations = 10, const u_int16_t &speed = 50,
                         const QString &name = "Random Z Lift",QObject *parent = Q_NULLPTR);

    virtual void createAnimation();
    u_int16_t getIterations() const
    {
        return m_iterations;
    }

public slots:
    void setIterations(u_int16_t &iterations)
    {
        if (m_iterations != iterations)
            m_iterations = iterations;
    }

private:
    u_int16_t m_iterations;
};

#endif // RANDOMZLIFT_HPP