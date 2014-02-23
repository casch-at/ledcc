#ifndef WALL_HPP
#define WALL_HPP
#include "Animation.hpp"

class Wall : public Animation
{
    Q_OBJECT
    Q_PROPERTY(Direction direction READ getDirection WRITE setDirection)
    Q_PROPERTY(Axis axis READ getAxis WRITE setAxis)
public:
    explicit Wall(const u_int16_t &speed = 50,
                  const Axis &axis = X_AXIS,
                  const Direction &direction = FORWARD,
                  const QString &name = "Wall",
                  QObject *parent = Q_NULLPTR);

    Direction getDirection(void) const{
        return m_direction;
    }
    Axis getAxis() const
    {
        return m_axis;
    }

    virtual void createAnimation();
public slots:
    void setDirection(Direction arg)
    {
        if(m_direction != arg)
            m_direction = arg;
    }

    void setAxis(Axis arg)
    {
        if(m_axis != arg)
            m_axis = arg;
    }
private:
    Direction m_direction;
    Axis m_axis;
};

#endif // WALL_HPP

