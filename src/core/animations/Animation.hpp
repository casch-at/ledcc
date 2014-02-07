#ifndef ANIMATION_HPP
#define ANIMATION_HPP
#include "Draw.hpp"

class Animation : public Draw
{
public:
    Animation(const QString &name,QObject *parent = Q_NULLPTR);

    void setName(const QString &n){
        if(name.compare(n) == 0){
            name = name;
        }
    }
    QString getName(void) const{
        return name;
    }
    void setSpeed(const u_int16_t &s){
        if(speed != s){
            speed = s;
        }
    }
    u_int16_t getSpeed(void) const{
        return speed;
    }

    void sendBixelZ(u_int8_t x, u_int8_t y, u_int8_t z, u_int16_t speed);
    void effectZUpDownMove(u_int8_t destination[CUBE_ARRAY_SIZE],
            u_int8_t position[CUBE_ARRAY_SIZE], Axis axe);
private:
    QString name;
    u_int16_t speed;
};

#endif // ANIMATION_HPP
