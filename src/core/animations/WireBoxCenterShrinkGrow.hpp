/**
 * Copyright (C) 2014  Christian Schwarzgruber <christiandev9@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
    virtual void createAnimationTooltipAsRichText(AnimationItem *item);
    virtual QStringList& getAnimationPropertiesAsPlainText( const AnimationItem *item );
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
