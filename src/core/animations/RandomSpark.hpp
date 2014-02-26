#ifndef RANDOMSPARK_HPP
#define RANDOMSPARK_HPP

#include "Animation.hpp"

class RandomSpark : public Animation
{
    Q_OBJECT
    Q_PROPERTY(u_int16_t sparks READ getSparks WRITE setSparks)
    Q_PROPERTY(u_int16_t iterations READ getIterations WRITE setIterations)
public:
    explicit RandomSpark(const u_int16_t &speed = 100,
                         const u_int16_t &iterations = 1,
                         const u_int16_t &sparks = 10,
                         const QString &name = "Random Spark",
                         QObject *parent = 0);
    u_int16_t getSparks() const
    {
        return m_sparks;
    }

    u_int16_t getIterations() const
    {
        return m_iterations;
    }

public Q_SLOTS:
    virtual void createAnimation();

    void setSparks(const u_int16_t &sparks)
    {
        if(m_sparks != sparks)
            m_sparks = sparks;
    }
    void setIterations(const u_int16_t &iterations)
    {
        if(m_iterations != iterations)
            m_iterations = iterations;
    }

private:
    void createSparks(const u_int16_t &leds);
    u_int16_t m_sparks;
    u_int16_t m_iterations;
};

#endif // RANDOMSPARK_HPP
