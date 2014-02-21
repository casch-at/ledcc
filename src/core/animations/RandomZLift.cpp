#include "RandomZLift.hpp"

RandomZLift::RandomZLift(const u_int16_t &iterations, const u_int16_t &speed, const QString &name, QObject *parent):
    Animation(speed,name,parent),
    m_iterations(iterations)
{
}

void RandomZLift::createAnimation()
{
    QVector<u_int8_t> destination(CUBE_ARRAY_SIZE);
    QVector<u_int8_t> position(CUBE_ARRAY_SIZE);

    for (u_int16_t i = 0; i < CUBE_ARRAY_SIZE; i++)
    {
        position[i] = 4;
        destination[i] = qrand() % 8;
    }

    for (u_int16_t i = 0; i < m_iterations; i++)
    {
        for (u_int8_t j = 0; j < CUBE_SIZE; j++)
        {
            effectZUpDownMove(destination, position, Z_AXIS);
            waitMs(getSpeed());
        }

        waitMs(getSpeed() * 4);

        for (u_int8_t j = 0; j < CUBE_ARRAY_SIZE / 2; j++)
        {
            destination[qrand() % CUBE_ARRAY_SIZE] = qrand() % CUBE_SIZE;
        }
    }
}
