#include "StringFly.hpp"

StringFly::StringFly(const u_int16_t &speed, const QString &string, const QString &name, QObject *parent) :
    Animation(speed,name,parent),
    m_sToDisplay(string)
{
}

void StringFly::createAnimation()
{
    u_int8_t x = 0;
    u_int8_t z = 0;
    u_int8_t i = 0;
    u_int8_t chr[5];

    for(int j = 0; j < m_sToDisplay.length(); j++)
    {
        fontGetChar(m_sToDisplay.at(j).toLatin1(), chr);

        // Put a character on the back of the cube
        for (x = 0; x < 5; x++)
        {
            for (z = 0; z < 8; z++)
            {
                if ((chr[x] & (0x80 >> z)))
                {
                    setBixel(x + 2, 7, z);
                }
            }
        }

        // Shift the entire contents of the cube forward by 6 steps
        // before placing the next character
        for (i = 0; i < 6; i++)
        {
            waitMs(getSpeed());
            shift(Y_AXIS, BACKWARD);
        }
    }
    // Shift the last character out of the cube.
    for (i = 0; i < 8; i++)
    {
        waitMs(getSpeed());
        shift(Y_AXIS, BACKWARD);
    }
    Q_EMIT done();
}
