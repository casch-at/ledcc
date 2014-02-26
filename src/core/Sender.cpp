#include "Sender.hpp"
#include <QSerialPort>
//#ifdef __DEBUG__
#include <QDebug>
//#endif

Sender::Sender(QObject *parent) :
    QObject(parent),
    m_stoped(false),
    m_running(false)
{
    m_serial = Q_NULLPTR;
}


void Sender::stop(void)
{
    if(isRunning())
        m_stoped = true;
}

void Sender::sendAnimation(const Draw::CubeArray &d)
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


void Sender::openCloseSerialPort(const SettingsDialog::SerialSettings &s)
{
    m_port = s;
    if(m_serial == Q_NULLPTR)
        m_serial = new QSerialPort;
    if(!m_serial->isOpen()){
        bool result = openSerialPort();
        if(result)
        {

            Q_EMIT portOpened(QString("Connected to %1 : %2, %3, %4, %5, %6")
                            .arg (m_port.name).arg (m_port.stringBaudRate)
                            .arg (m_port.stringDataBits).arg (m_port.stringParity)
                            .arg (m_port.stringStopBits).arg (m_port.stringFlowControl));
        }else
        {
            Q_EMIT portError(QString(tr("Can't open serial port: <b>%1</b> - error code: <b>%2</b>#Check if device is connected properly"))
                             .arg(m_port.name)
                             .arg(m_serial->error()));
            m_serial->close();
        }
    }
    else{
        Q_EMIT closePort(QString(tr("Do you really want close the serial port: <b>%1</b>")
                                 .arg(m_port.name)));
    }
}


bool Sender::checkPortSettings(void)
{
    if( m_serial->setBaudRate (m_port.baudRate) && m_serial->setDataBits (m_port.dataBits)
            && m_serial->setParity (m_port.parity) && m_serial->setStopBits (m_port.stopBits)
            && m_serial->setFlowControl (m_port.flowControl))
    {
        return true;
    }else
    {
        Q_EMIT portError(QString(tr("Can't configure the serial port: <b>%1</b>,\n"
                                    "error code: <b>%2</b>"))
                         .arg (m_port.name).arg (m_serial->error ()));
        return false;
    }
}
/**
 * @brief MainWindow::closeSerialPort
 */
void Sender::closeSerialPort(void)
{
    m_serial->close();
    Q_EMIT portClosed(QString("Port closed: %1").arg(m_port.name));
    qDebug("Closed");
}
/**
 * @brief MainWindow::openSerialPort
 * @return
 */
bool Sender::openSerialPort(void)
{
    bool result;

    m_serial->setPortName (m_port.name);

    if(m_serial->open(QIODevice::ReadWrite))
    {
        result = checkPortSettings();

    }else
    {
        result = false;
    }
    return result;
}