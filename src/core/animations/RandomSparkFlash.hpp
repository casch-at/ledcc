#ifndef RANDOMSPARKFLASH_HPP
#define RANDOMSPARKFLASH_HPP

#include "Animation.hpp"

class RandomSparkFlash : public Animation
{
    Q_OBJECT
    Q_PROPERTY(u_int16_t iterations READ getIterations WRITE setIterations)
    Q_PROPERTY(u_int16_t leds READ getLeds WRITE setLeds)
public:
    explicit RandomSparkFlash(const u_int16_t &speed = 40,
                              const u_int16_t &iterations = 20,
                              const u_int16_t &leds = 40,
                              const QString &name = "Random Spark Flash",
                              QObject *parent = 0);

    u_int16_t getIterations() const
    {
        return m_iterations;
    }

    u_int16_t getLeds() const
    {
        return m_leds;
    }

signals:

public Q_SLOTS:
    virtual void createAnimation();

    void setIterations(const u_int16_t iterations)
    {
        if(m_iterations != iterations)
            m_iterations = iterations;
    }
    void setLeds(const u_int16_t leds)
    {
        if(m_leds != leds)
            m_leds = leds;
    }
private:
    u_int16_t m_iterations;
    u_int16_t m_leds;
};

#endif // RANDOMSPARKFLASH_HPP
