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
#ifndef RANDOMSPARK_HPP
#define RANDOMSPARK_HPP

#include "Animation.hpp"


class RandomSpark : public Animation
{
    Q_OBJECT
    Q_PROPERTY(quint16 sparks READ getSparks WRITE setSparks)
    Q_PROPERTY(quint16 iterations READ getIterations WRITE setIterations)
public:
    explicit RandomSpark(const quint16 &speed = 100,
                         const quint16 &iterations = 1,
                         const quint16 &sparks = 10,
                         const QString &name = BIAS::RandomSpark,
                         QObject *parent = 0);
    quint16 getSparks() const
    {
        return m_sparks;
    }

    quint16 getIterations() const
    {
        return m_iterations;
    }

public Q_SLOTS:
    virtual void createAnimation();
    void setSparks(const quint16 &sparks)
    {
        if(m_sparks != sparks)
            m_sparks = sparks;
    }
    void setIterations(const quint16 &iterations)
    {
        if(m_iterations != iterations)
            m_iterations = iterations;
    }


private:
    void createSparks(const quint16 &leds);
    quint16 m_sparks;
    quint16 m_iterations;
};

#endif // RANDOMSPARK_HPP
