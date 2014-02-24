#include "Rain.hpp"


Rain::Rain(const u_int16_t &iterations, const u_int16_t &speed, const QString &name, QObject *parent):
    Animation(speed,name,parent),
    m_iterations(iterations)
{
}

void Rain::createAnimation()
{
    u_int8_t i = 0;
    u_int16_t ii = 0;

    for (ii = 0; ii < m_iterations; ii++)
    {
        for (i = 0; i < qrand() % (CUBE_SIZE / 2); i++)
        {
            setBixel(qrand() % CUBE_SIZE, qrand() % CUBE_SIZE, 7);
        }

        waitMs(getSpeed());
        shift(Z_AXIS, BACKWARD);
    }
    Q_EMIT done();
}
