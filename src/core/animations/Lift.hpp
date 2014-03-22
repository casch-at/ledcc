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
#ifndef LIFT_HPP
#define LIFT_HPP
#include "Animation.hpp"


class Lift : public Animation
{
    Q_OBJECT
    Q_PROPERTY(u_int16_t iterations READ getIterations WRITE setIterations)
    Q_PROPERTY(u_int16_t delay READ getDelay WRITE setDelay)
public:
   explicit Lift(const u_int16_t &delay = 50, const u_int16_t  &iterations = 10,
         const QString &name = "Lift", const u_int16_t &speed = 80, QObject *parent = Q_NULLPTR);
    u_int16_t getIterations() const
    {
        return m_iterations;
    }

    u_int16_t getDelay() const
    {
        return m_delay;
    }

public Q_SLOTS:
    virtual void createAnimation();
    virtual const QString createAnimationTooltip();
    virtual void createAnimationTooltip(AnimationItem *item);
    void setIterations(const u_int16_t &arg)
    {
        if(m_iterations != arg)
            m_iterations = arg;
    }

    void setDelay(const u_int16_t &arg)
    {
        if(m_delay != arg)
            m_delay = arg;
    }

private:
    u_int16_t m_delay;
    u_int16_t m_iterations;
};

#endif // LIFT_HPP
