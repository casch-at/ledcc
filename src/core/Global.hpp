#ifndef GLOBAL_H
#define GLOBAL_H
#include "stdint.h"

#define CUBE_SIZE                       8
#define IT_CUBE_SIZE                    (CUBE_SIZE - 1)
#define CUBE_ARRAY_SIZE                 (CUBE_SIZE*CUBE_SIZE)
#define CUBE_LED_COUNT                  (CUBE_ARRAY_SIZE*CUBE_SIZE)
#include <QVector>
#include <QString>
#include <QObject>
/************************************************************************
 *                   ENUMERATIONS
 ************************************************************************/

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;
typedef QVector<QVector<uint8_t> > CubeArray;



/** @brief: BOOLEAN TYPE */
typedef enum
{
    FALSE = 0,
    TRUE = !FALSE
}Bool;

/** @brief: AXIS TYPE */
typedef enum
{
    X_AXIS = 0,
    Y_AXIS,
    Z_AXIS
} Axis;

/** @brief: DIRECTION TYPE */
typedef enum
{
    BACKWARED = 0,
    FORWARED = !BACKWARED
} Direction;

/** @brief: BIXEL STATE */
typedef enum
{
    OFF = 0,
    ON = !OFF
} BixelState;

typedef struct{
    QString name;
    QString text;
    uint8_t id;
    uint8_t particle;
    uint16_t speed;
    uint16_t delay;
    uint16_t leds;
    uint16_t iteration;
    Direction direction;
    Axis axis;
    Bool invert;
    BixelState state;
}AnimationStruct;
Q_DECLARE_METATYPE(AnimationStruct)
#endif // GLOBAL_H
