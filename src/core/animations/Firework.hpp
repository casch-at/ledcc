#ifndef FIREWORK_HPP
#define FIREWORK_HPP

#include "Animation.hpp"

class Firework : public Animation
{
    Q_OBJECT
    Q_PROPERTY(u_int8_t iterations READ getIterations WRITE setIterations)
    Q_PROPERTY(u_int16_t particles READ getParticles WRITE setParticles)
public:
    explicit Firework(const u_int16_t &iterations = 15, const u_int16_t &speed = 50,
                      const u_int8_t &particles = 10, const QString &name = "Firework",
                      QObject *parent = Q_NULLPTR);

    u_int8_t getIterations() const
    {
        return m_iterations;
    }

    u_int16_t getParticles() const
    {
        return m_particles;
    }

public Q_SLOTS:
    virtual void createAnimation();

    void setIterations(const u_int8_t &iterations)
    {
        if(m_iterations != iterations)
            m_iterations = iterations;
    }

    void setParticles(const u_int16_t &particles)
    {
        if(m_particles != particles)
            m_particles = particles;
    }

private:
    u_int16_t m_iterations;
    u_int8_t m_particles;
};

#endif // FIREWORK_HPP
