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

#ifndef WAVE_HPP
#define WAVE_HPP

#include "Animation.hpp"

class Wave : public Animation
{
    Q_OBJECT
    Q_PROPERTY(quint16 iterations READ iterations WRITE setIterations)

public:
    explicit Wave(const quint16 &speed = 40,
                  const quint16 &iterations = 20,
                  const QString &name = BIAS::Wave,
                  QObject *parent = 0);

    quint16 iterations() const
    {
        return m_iterations;
    }

signals:

public Q_SLOTS:
    void createAnimation();

    void setIterations(quint16 arg)
    {
        m_iterations = arg;
    }
private:
    quint16 m_iterations;
};

#endif // WAVE_HPP
