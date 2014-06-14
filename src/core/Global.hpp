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
#ifndef GLOBAL_H
#define GLOBAL_H
#include "stdint.h"
#include <QVector>
#include <QString>

static const quint8 AVAILABLEANIMATIONS = 15;
static const quint8 CUBE_SIZE = 8;
static const quint8 IT_CUBE_SIZE = CUBE_SIZE - 1;
static const quint8 CUBE_ARRAY_SIZE = CUBE_SIZE * CUBE_SIZE;
static const quint32  CUBE_LED_COUNT = CUBE_ARRAY_SIZE * CUBE_SIZE;

#define LEDCC_INSTALL_PATH  "/home/christian/ledcc/share/ledcc"
#define LEDCC_VERSION_MAJOR 0
#define LEDCC_VERSION_MINOR 1
//#define LEDCC_VERSION_MICRO 
#define LEDCC_VERSION       0.1

#endif // GLOBAL_H
