#ifndef ANIMATIONS_HPP
#define ANIMATIONS_HPP
#include "Global.hpp"
#include "Draw.hpp"
#include <QByteArray>
#include <QThread>
#include <QReadWriteLock>

class QTimer;
//class QSerialPort;

class Animations : public Draw
{
    Q_OBJECT
public:
    Animations(QByteArray *byteArray);
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
    void effectWireBoxCenterShrinkGrow(uint16_t speed, Bool centerStart);
    void effectAxisNailWall(uint16_t speed, Axis axis, Bool invert);
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
//    void dataChanged(const CubeArray &cubeFTemp);
    void dataChanged(const CubeArray &cubeF);
    void sleepAnimation(const uint &ms);

private:
    bool tmp;
    void animationWait(const uint speed);
    void sendBixelZ(uint8_t x, uint8_t y, uint8_t z, uint16_t speed);
    void effectZUpDownMove(uint8_t destination[CUBE_ARRAY_SIZE],
            uint8_t position[CUBE_ARRAY_SIZE], Axis axe);

    QTimer *timer;
    QReadWriteLock lock;
};
#endif // ANIMATIONS_HPP
