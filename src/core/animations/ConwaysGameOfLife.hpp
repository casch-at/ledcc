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

#ifndef CONWAYSGAMEOFLIFE_HPP
#define CONWAYSGAMEOFLIFE_HPP

#include "Animation.hpp"

class ConwaysGameOfLife : public Animation
{
    Q_OBJECT
    Q_PROPERTY(quint16 iterations READ iterations WRITE setIterations)
    Q_PROPERTY(bool invert READ invert WRITE setInvert)
public:
    explicit ConwaysGameOfLife(const quint16 &speed = 120,
                               const QString &name = BIAS::ConwaysGameOfLife,
                               const quint16 &iterations = 100,
                               const bool invert = true,
                               QObject *parent = 0);

    quint16 iterations() const
    {
        return m_iterations;
    }

    bool invert() const
    {
        return m_invert;
    }

public Q_SLOTS:
    void createAnimation();

    void setIterations(const quint16 &iterations)
    {
        if(m_iterations != iterations)
            m_iterations = iterations;
    }

    void setInvert(const bool invert)
    {
        if (m_invert != invert) {
            m_invert = invert;
        }
    }

private:
    static const quint8 TERMINATE_LONELY = 3;
    static const quint8 TERMINATE_CROWDED = 5;
    static const quint8 CREATE_MIN = 4;
    static const quint8 CREATE_MAX = 4;

    quint16 countChanges();
    quint8 countNeighbors(quint8 x, quint8 y, quint8 z);
    void nextgen();

    quint16 m_iterations;
    bool m_invert;
};

#endif // CONWAYSGAMEOFLIFE_HPP
