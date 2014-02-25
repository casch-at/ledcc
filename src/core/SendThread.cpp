#include "SendThread.hpp"
#include <QSerialPort>
#include <QDebug>

SendThread::SendThread(QObject *parent) :
    QObject(parent),
    m_serial(new QSerialPort),
    m_stoped(false),
    m_running(false)
{

}



void SendThread::stop(void)
{
    if(isRunning())
        m_stoped = true;
}

void SendThread::sendAnimations(Draw::CubeArray &d)
{
    m_serial->putChar(0xFF);
    m_serial->waitForBytesWritten(1000);
    m_serial->putChar(0x00);
    m_serial->waitForBytesWritten(1000);
    for (u_int8_t z = 0; z < CUBE_SIZE; z++) {
        for (u_int8_t y = 0; y < CUBE_SIZE; y++) {
            m_serial->putChar(d[z][y]);
            m_serial->waitForBytesWritten(1000);
            if(d[z][y] == 0xFF){
                m_serial->putChar(0xFF);
                m_serial->waitForBytesWritten(1000);
            }
        }
    }
}
