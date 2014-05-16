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
public:
    explicit ConwaysGameOfLife(const quint16 &speed = 30,
                               const QString &name = BIAS::ConwaysGameOfLife,
                               QObject *parent = 0);

public Q_SLOTS:
    void createAnimation();
private:

};

#endif // CONWAYSGAMEOFLIFE_HPP
