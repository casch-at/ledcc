#ifndef WALL_HPP
#define WALL_HPP
#include "Animation.hpp"

class Wall : public Animation
{
    Q_OBJECT
    Q_PROPERTY(Direction direction READ getDirection WRITE setDirection)
    Q_PROPERTY(Axis axis READ getAxis WRITE setAxis)
public:
    explicit Wall(const Axis &axis,const Direction &direction,
                  const u_int16_t &speed, QObject *parent = Q_NULLPTR,
                  const QString &name = "Wall");

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

