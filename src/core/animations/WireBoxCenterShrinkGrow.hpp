#ifndef WIREBOXCENTERSHRINKGROW_HPP
#define WIREBOXCENTERSHRINKGROW_HPP

#include "Animation.hpp"

class WireBoxCenterShrinkGrow : public Animation
{
    Q_OBJECT
    Q_PROPERTY(bool centerStart READ getCenterStart WRITE setCenterStart)
public:
    explicit WireBoxCenterShrinkGrow(const u_int16_t &speed = 50,
                                     const bool centerStart = false,
                                     const QString &name = "Wire Box Center Shrink Grow",
                                     QObject *parent = Q_NULLPTR);
    bool getCenterStart() const
    {
        return m_centerStart;
    }

signals:

public Q_SLOTS:
    virtual void createAnimation();
    void setCenterStart(bool centerStart)
    {
        if(m_centerStart != centerStart)
        m_centerStart = centerStart;
    }
private:
    bool m_centerStart;
};

#endif // WIREBOXCENTERSHRINKGROW_HPP
