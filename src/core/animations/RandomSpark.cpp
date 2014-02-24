#include "RandomSpark.hpp"
#include "RandomSparkFlash.hpp"

RandomSpark::RandomSpark(const u_int16_t &speed, const u_int16_t &iterations, const u_int16_t &sparks,
                         const QString &name, QObject *parent):
    RandomSparkFlash(speed,iterations,0,name,parent),
    m_sparks(sparks)
//    Animation(speed,iterations,parent)
{

}

void RandomSpark::createAnimation()
{
    for (u_int16_t i = 0; i < m_sparks; i++)
    {
        setLeds(i);
        RandomSparkFlash::createAnimation();
    }
    for (u_int16_t i = m_sparks; i >= 1; i--)
    {
        setLeds(i);
        RandomSparkFlash::createAnimation();
    }
    Q_EMIT done();
}
