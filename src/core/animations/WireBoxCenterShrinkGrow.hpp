#ifndef WIREBOXCENTERSHRINKGROW_HPP
#define WIREBOXCENTERSHRINKGROW_HPP

#include "Animation.hpp"

class WireBoxCenterShrinkGrow : public Animation
{
    Q_OBJECT
    Q_PROPERTY(u_int8_t name READ name WRITE setName NOTIFY nameChanged)
public:
    explicit WireBoxCenterShrinkGrow(QObject *parent = 0);
    virtual void
signals:

public slots:

private:
};

#endif // WIREBOXCENTERSHRINKGROW_HPP
