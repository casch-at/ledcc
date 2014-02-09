#ifndef GLOBAL_H
#define GLOBAL_H
#include "stdint.h"
#include <QVector>
#include <QString>
#include <QObject>

/************************************************************************
 *                      DEFINES
 ************************************************************************/
#define CUBE_SIZE                       8
#define IT_CUBE_SIZE                    (CUBE_SIZE - 1)
#define CUBE_ARRAY_SIZE                 (CUBE_SIZE*CUBE_SIZE)
#define CUBE_LED_COUNT                  (CUBE_ARRAY_SIZE*CUBE_SIZE)


/************************************************************************
 *                   TYPE DEFENITIONS
 ************************************************************************/
//typedef unsigned char uint8_t;
//typedef unsigned short uint16_t;
//typedef unsigned int uint32_t;
//typedef unsigned long int uint64_t;

/************************************************************************
 *     ANIMATION STRUCT REPRESENTING THE AVAILABLE PARAMETERS
 ************************************************************************/
//typedef struct{
//    uint8_t id;
//    QString name;
//    QString text;
//    uint8_t particle;
//    uint16_t speed;
//    uint16_t delay;
//    uint16_t leds;
//    uint16_t iteration;
//    Direction direction;
//    Axis axis;
//    bool invert;
//    BixelState state;
//}AnimationStruct;
//Q_DECLARE_METATYPE(AnimationStruct)

#endif // GLOBAL_H
