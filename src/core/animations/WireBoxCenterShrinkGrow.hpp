#ifndef WIREBOXCENTERSHRINKGROW_HPP
#define WIREBOXCENTERSHRINKGROW_HPP

#include "Animation.hpp"

class WireBoxCenterShrinkGrow : public Animation
{
    Q_OBJECT
    Q_PROPERTY(bool centerStart READ getCenterStart WRITE setCenterStart)
    Q_PROPERTY(u_int16_t iterations READ getIterations WRITE setIterations)
public:
    explicit WireBoxCenterShrinkGrow(const u_int16_t &speed = 180,
                                     const bool centerStart = false,
                                     const u_int16_t &iterations = 1,
                                     const QString &name = "Wire Box Center Shrink Grow",
                                     QObject *parent = Q_NULLPTR);
    bool getCenterStart() const
    {
        return m_centerStart;
    }

    u_int16_t getIterations() const
    {
        return m_iterations;
    }

signals:

public Q_SLOTS:
    virtual void createAnimation();
    void setCenterStart(bool centerStart)
    {
        if(m_centerStart != centerStart)
            m_centerStart = centerStart;
    }
    void setIterations(const u_int16_t &iterations)
    {
        if(m_iterations != iterations )
            m_iterations = iterations;
    }

private:
    bool m_centerStart;
    u_int16_t m_iterations;
};

#endif // WIREBOXCENTERSHRINKGROW_HPP
