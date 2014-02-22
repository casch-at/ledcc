#include "RandomFiller.hpp"

RandomFiller::RandomFiller(const u_int16_t &speed, const BixelState &state, const QString &name, QObject *parent) :
    Animation(speed,name,parent),
    m_state(state)
{
}

void RandomFiller::createAnimation()
{
    u_int32_t leds = 0;
    u_int8_t x = 0;
    u_int8_t y = 0;
    u_int8_t z = 0;

    if (m_state)
        fillCubeArray(0x00);
    else
        fillCubeArray(0xFF);

    while (leds < CUBE_LED_COUNT - 1)
    {
        x = qrand() % CUBE_SIZE;
        y = qrand() % CUBE_SIZE;
        z = qrand() % CUBE_SIZE;
        if ((m_state == OFF && getBixelState(x, y, z) == ON)
                || (m_state == ON && getBixelState(x, y, z) == OFF))
        {
            alterBixel(x, y, z, m_state);
            waitMs(getSpeed());
            leds++;
        }
    }
}
