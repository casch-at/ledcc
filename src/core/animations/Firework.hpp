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
#ifndef FIREWORK_HPP
#define FIREWORK_HPP

#include "Animation.hpp"

class Firework : public Animation
{
    Q_OBJECT
    Q_PROPERTY(quint16 iterations READ iterations WRITE setIterations)
    Q_PROPERTY(quint16 particles READ particles WRITE setParticles)
public:
    explicit Firework(const quint16 &iterations = 15,
                      const quint16 &speed = 50,
                      const quint8 &particles = 20,
                      const QString &name = BIAS::Firework,
                      QObject *parent = Q_NULLPTR);

    quint16 iterations() const
    {
        return m_iterations;
    }

    quint16 particles() const
    {
        return m_particles;
    }

public Q_SLOTS:
    virtual void createAnimation();
    void setIterations(const quint16 &iterations)
    {
        if(m_iterations != iterations)
            m_iterations = iterations;
    }

    void setParticles(const quint16 &particles)
    {
        if(m_particles != particles)
            m_particles = particles;
    }
private:
    quint16 m_iterations;
    quint8 m_particles;
};
#endif // FIREWORK_HPP
