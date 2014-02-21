#ifndef LOADBAR_HPP
#define LOADBAR_HPP

#include "Animation.hpp"

class Loadbar : public Animation
{
    Q_OBJECT
    Q_PROPERTY(Axis axis READ getAxis WRITE setAxis)
public:
    explicit Loadbar(const u_int16_t &speed = 50, const Axis &axis = X_AXIS,
                     const QString &name = "Loadbar", QObject *parent = 0);
    Axis getAxis() const
    {
        return m_axis;
    }

    virtual void createAnimation();
signals:

public slots:

    void setAxis(const Axis axis)
    {
        if(m_axis != axis)
            m_axis = axis;
    }

private:

    Axis m_axis;
};

#endif // LOADBAR_HPP
