#ifndef RANDOMSPARK_HPP
#define RANDOMSPARK_HPP

#include "Animation.hpp"
#include "RandomSparkFlash.hpp"

class RandomSpark : public RandomSparkFlash
{
    Q_OBJECT
    Q_PROPERTY(u_int16_t sparks READ getSparks WRITE setSparks)
public:
    explicit RandomSpark(const u_int16_t &speed = 50,
                         const u_int16_t &iterations = 5,
                         const u_int16_t &sparks = 20,
                         const QString &name = "Random Spark",
                         QObject *parent = 0);
    u_int16_t getSparks() const
    {
        return m_sparks;
    }

public Q_SLOTS:
    virtual void createAnimation();

    void setSparks(const u_int16_t sparks)
    {
        if(m_sparks != sparks)
            m_sparks = sparks;
    }
private:
    u_int16_t m_sparks;
};

#endif // RANDOMSPARK_HPP
