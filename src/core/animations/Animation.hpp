#ifndef ANIMATION_HPP
#define ANIMATION_HPP
#include "Draw.hpp"
#include <QTimer>

class Animation : public Draw
{
    Q_OBJECT
public:
    explicit  Animation(const u_int16_t &speed, const QString &name,QObject *parent = Q_NULLPTR):
        Draw(parent),
        m_speed(speed),
        m_name(name),
        m_timer(new QTimer)
    {
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
    void effectZUpDownMove(QVector<u_int8_t> &destination,
            QVector<u_int8_t> &position, Axis axe);

    void waitMs(const u_int16_t &time);
Q_SIGNALS:
    void done();
    void sendData();
public Q_SLOTS:
    virtual void createAnimation(void) = 0;
private:
    u_int16_t m_speed;
    QString m_name;
    QTimer *m_timer;
};

#endif // ANIMATION_HPP
