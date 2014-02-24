#ifndef RANDOMFILLER_HPP
#define RANDOMFILLER_HPP

#include "Animation.hpp"

class RandomFiller : public Animation
{
    Q_OBJECT
    Q_PROPERTY(BixelState state READ getState WRITE setState)
public:
    explicit RandomFiller(const u_int16_t &speed = 50,
                          const BixelState &state = OFF,
                          const QString &name = "Random Filler",
                          QObject *parent = 0);
    BixelState getState() const
    {
        return m_state;
    }

signals:

public Q_SLOTS:
    virtual void createAnimation();
    void setState(const BixelState state)
    {
        if(m_state != state)
            m_state = state;
    }

private:
    BixelState m_state;
};

#endif // RANDOMFILLER_HPP
