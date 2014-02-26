#ifndef ANIMATION_HPP
#define ANIMATION_HPP
#include "Draw.hpp"
#include <QDebug>
class Animation : public Draw
{
    Q_OBJECT
public:
    explicit  Animation(const u_int16_t &speed, const QString &name,QObject *parent = Q_NULLPTR):
        Draw(parent),
        m_speed(speed),
        m_name(name)
    {
        m_abort = false;
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
    void effectZUpDownMove(QVector<u_int8_t> &destination,
            QVector<u_int8_t> &position, Axis axe);

    void waitMs(const u_int16_t &time);
    bool m_abort;
Q_SIGNALS:
    void done();
    void sendData(const CubeArray &cubeFrame);
public Q_SLOTS:
    virtual void createAnimation(void) = 0;
private:
    u_int16_t m_speed;
    QString m_name;
};

#endif // ANIMATION_HPP
