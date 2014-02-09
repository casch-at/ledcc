#include "Lift.hpp"

void Lift::createAnimation()
{
    u_int8_t lastX = 0;
    u_int8_t lastY = 0;
    u_int8_t x;
    u_int8_t y;

    for (x = 0; x < CUBE_SIZE; x++)
    {
        for (y = 0; y < CUBE_SIZE; y++)
        {
            setBixel(x, y, (qrand() % 2) * 7);
        }
    }

    for (u_int16_t i = 0; i < m_iterations; i++)
    {
        x = qrand() % CUBE_SIZE;
        y = qrand() % CUBE_SIZE;

        if (y != lastY && x != lastX)
        {
            if (getBixelState(x, y, 0))
                sendBixelZ(x, y, 0, getSpeed());
            else
                sendBixelZ(x, y, 7, getSpeed());
            lastX = x;
            lastY = y;
        }
    }
}
