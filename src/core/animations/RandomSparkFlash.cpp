#include "RandomSparkFlash.hpp"

RandomSparkFlash::RandomSparkFlash(const u_int16_t &speed, const u_int16_t &iterations, const u_int16_t &leds, const QString &name, QObject *parent):
    Animation(speed,name,parent),
    m_iterations(iterations),
    m_leds(leds)
{
}

void RandomSparkFlash::createAnimation()
{
    for (u_int16_t i = 0; i < m_iterations; i++)
    {
        for (u_int16_t b = 0; b < m_leds; b++)
        {
            setBixel(qrand() % CUBE_SIZE, qrand() % CUBE_SIZE,
                    qrand() % CUBE_SIZE);
        }
        waitMs(getSpeed());
        fillCubeArray(0x00);
    }
    Q_EMIT done();
}
