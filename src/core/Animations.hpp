#ifndef ANIMATIONS_HPP
#define ANIMATIONS_HPP
#include "Global.hpp"
#include "animations/Draw.hpp"
#include <QByteArray>
#include <QThread>
#include <QReadWriteLock>

class QTimer;

class Animations : public Draw
{
    Q_OBJECT
public:
    Animations();
    ~Animations();


//    QByteArray getSendCube(void);
    /************************************************************
     *                  EFFECTS FUNCTION PROTOTYPES
     ************************************************************/
    void effectWall(Axis axis, Direction direction, uint16_t speed);
    void effectTest(void);
    void effectRain(uint16_t iterations,uint16_t speed);
    void effectLift(uint16_t iterations, uint16_t speed, uint16_t delay);
    void effectRandomZLift(uint16_t iterations, uint16_t speed);
    void effectFirework(uint16_t iterations, uint16_t speed, uint8_t particles);
    void effectWireBoxCornerShrinkGrow(uint16_t iterations, uint16_t speed, uint8_t rotate,
            uint8_t flip);
    void effectWireBoxCenterShrinkGrow(uint16_t speed, bool centerStart);
    void effectAxisNailWall(uint16_t speed, Axis axis, bool invert);
    void effectLoadbar(uint16_t speed, Axis axis);
    void effectRandomSparkFlash(uint16_t iterations, uint16_t speed,
            uint16_t bixels);
    void effectRandomSpark(uint16_t sparks, uint16_t speed);
    void effectRandomFiller(uint16_t speed, BixelState state);
    void effectString(uint8_t *str, uint16_t speed);
//    void ripples (int iterations, int delay);
    //float distance2d (float x1, float y1, float x2, float y2);

public slots:

signals:

private:
    void animationWait(const uint speed);
};
#endif // ANIMATIONS_HPP
