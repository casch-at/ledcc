#ifndef WIREBOXCENTERSHRINKGROW_HPP
#define WIREBOXCENTERSHRINKGROW_HPP

#include "Animation.hpp"

class WireBoxCenterShrinkGrow : public Animation
{
    Q_OBJECT
    Q_PROPERTY(bool centerStart READ getCenterStart WRITE setCenterStart)
public:
    explicit WireBoxCenterShrinkGrow(const u_int16_t &speed,
                                     const bool centerStart,
                                     const QString &name,
                                     QObject *parent = 0);
    virtual void createAnimation();
    bool getCenterStart() const
    {
        return m_centerStart;
    }

signals:

public slots:
    void setCenterStart(bool centerStart)
    {
        if(m_centerStart != centerStart)
        m_centerStart = centerStart;
    }
private:
    bool m_centerStart;
};

#endif // WIREBOXCENTERSHRINKGROW_HPP
