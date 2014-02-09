#ifndef ANIMATION_HPP
#define ANIMATION_HPP
#include "Draw.hpp"

class Animation : public Draw
{
public:
    Animation(const QString &name,QObject *parent = Q_NULLPTR):
        Draw(),
        m_name(name){
    }

    void setName(const QString &name){
        if(m_name.compare(name) == 0){
            m_name = name;
        }
    }
    QString getName(void) const{
        return m_name;
    }
    void setSpeed(const u_int16_t &speed){
        if(m_speed != speed){
            m_speed = speed;
        }
    }
    u_int16_t getSpeed(void) const{
        return m_speed;
    }

    void sendBixelZ(u_int8_t x, u_int8_t y, u_int8_t z, u_int16_t speed);
    void effectZUpDownMove(u_int8_t destination[CUBE_ARRAY_SIZE],
            u_int8_t position[CUBE_ARRAY_SIZE], Axis axe);
    virtual void createAnimation(void) = 0;
private:
    u_int16_t m_speed;
    QString m_name;
};

#endif // ANIMATION_HPP
