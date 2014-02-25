#ifndef WIREBOXCORNERSHRINKGROW_HPP
#define WIREBOXCORNERSHRINKGROW_HPP

#include "Animation.hpp"

class WireBoxCornerShrinkGrow : public Animation
{
    Q_OBJECT
    Q_PROPERTY(u_int16_t iterations READ getIterations WRITE setIterations)
    Q_PROPERTY(u_int8_t rotate READ getRotate WRITE setRotate)
    Q_PROPERTY(u_int8_t flip READ getFlip WRITE setFlip)
public:
    explicit WireBoxCornerShrinkGrow(const u_int16_t &iterations = 1,
                                     const u_int16_t &speed = 50,
                                     const u_int8_t &rotate = 1,
                                     const u_int8_t &flip=0,
                                     const QString &name = "Wire Box Corner Shrink Grow",
                                     QObject *parent = Q_NULLPTR);


    u_int16_t getIterations() const
    {
        return m_iterations;
    }

    u_int8_t getRotate() const
    {
        return m_rotate;
    }

    u_int8_t getFlip() const
    {
        return m_flip;
    }

public Q_SLOTS:
    virtual void createAnimation();

    void setIterations(const u_int16_t &iterations)
    {
        if(m_iterations != iterations)
            m_iterations = iterations;
    }

    void setRotate(const u_int8_t &rotate)
    {
        if(m_rotate != rotate)
            m_rotate = rotate;
    }

    void setFlip(const u_int8_t &flip)
    {
        if(m_flip != flip)
            m_flip = flip;
    }

private:
    void createWireBoxCorner(const u_int8_t rotate, const u_int8_t flip);
    u_int16_t m_iterations;
    u_int8_t m_rotate;
    u_int8_t m_flip;
};

#endif // WIREBOXCORNERSHRINKGROW_HPP
