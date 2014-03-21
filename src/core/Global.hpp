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
//#include <QObject>

/************************************************************************
 *                      CONSTANTS
 ************************************************************************/
static const u_int8_t AVAILABLEANIMATIONS = 13;
static const u_int8_t CUBE_SIZE = 8;
static const u_int8_t IT_CUBE_SIZE = CUBE_SIZE - 1;
static const u_int8_t CUBE_ARRAY_SIZE = CUBE_SIZE * CUBE_SIZE;
static const u_int CUBE_LED_COUNT = CUBE_SIZE * CUBE_SIZE * CUBE_SIZE;

/************************************************************************
 *     ANIMATION STRUCT REPRESENTING THE AVAILABLE PARAMETERS
 ************************************************************************/

#define LED_CUBE_VERSION_MAJOR 0.1
#define LED_CUBE_VERSION_MINOR 0.11

#endif // GLOBAL_H
