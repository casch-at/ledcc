#include "RandomSpark.hpp"
#include "RandomSparkFlash.hpp"

RandomSpark::RandomSpark(const u_int16_t &speed, const u_int16_t &iterations, const u_int16_t &sparks,
                         const QString &name, QObject *parent):
    Animation(speed,name,parent),
    m_sparks(sparks),
    m_iterations(iterations)
{

}

void RandomSpark::createAnimation()
{
    for (u_int16_t i = 0; i < m_sparks; i++)
    {
        createSparks(i);
    }
    for (u_int16_t i = m_sparks; i >= 1; i--)
    {
        createSparks(i);
    }
    Q_EMIT done();
}

void RandomSpark::createSparks(const u_int16_t &leds)
{
    for (u_int16_t i = 0; i < m_iterations; i++)
    {
        for (u_int16_t b = 0; b < leds; b++)
        {
            setBixel(qrand() % CUBE_SIZE, qrand() % CUBE_SIZE,
                    qrand() % CUBE_SIZE);
        }
        waitMs(getSpeed());
        fillCubeArray(0x00);
    }
}

