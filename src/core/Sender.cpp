/**
 * Copyright (C) 2014  Christian Schwarzgruber <christiandev9@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Sender.hpp"
#include <QSerialPort>
#ifdef _DEBUG_
#include <QDebug>
#endif

/**
 * @brief Sender constructor
 *
 * @param parent
 */
Sender::Sender(QObject *parent) :
    QObject(parent),
    m_abort(false)
{
    m_portOpen = false;
    m_serial = Q_NULLPTR;
}

/**
 * @brief Sender deconstructor
 *
 */
Sender::~Sender()
{
    if(m_serial != Q_NULLPTR )
        if(m_serial->isOpen())
            m_serial->close();
    delete m_serial;
}

/**
 * @brief
 *
 * @param d
 */
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
            if(m_abort)
                return;
            if(d[z][y] == 0xFF){
                m_serial->putChar(0xFF);
                m_serial->waitForBytesWritten(1000);
            }
        }
    }
}


/**
 * @brief Function to open and close serial port
 *
 * @param s Serial port configurations
 */
void Sender::openCloseSerialPort()
{
    if(m_serial == Q_NULLPTR)
        m_serial = new QSerialPort;
    if(!m_serial->isOpen()){
        bool result = openSerialPort();
        if(result)
        {

            Q_EMIT portOpened(QString("Connected to %1 : %2, %3, %4, %5, %6")
                              .arg (serialSettings()->m_name)
                              .arg (serialSettings()->m_stringBaudRate)
                              .arg (serialSettings()->m_stringDataBits)
                              .arg (serialSettings()->m_stringParity)
                              .arg (serialSettings()->m_stringStopBits)
                              .arg (serialSettings()->m_stringFlowControl));
            setPortOpen(true);
        }else
        {
            Q_EMIT portError(QString(tr("Can't open serial port: <b>%1</b> - error code: <b>%2</b>#Check if device is connected properly"))
                             .arg(serialSettings()->m_name)
                             .arg(m_serial->error()));
            m_serial->close();
            setPortOpen(false);
        }
    }
    else{
        Q_EMIT closePort(QString(tr("Do you really want close the serial port: <b>%1</b>")
                                 .arg(serialSettings()->m_name)));
    }
}


/**
 * @brief Checks the port settings and
 *        returns true if they are ok otherwise false
 *
 * @return bool
 */
bool Sender::checkPortSettings(void)
{
    if( m_serial->setBaudRate (serialSettings()->m_baudRate) && m_serial->setDataBits (serialSettings()->m_dataBits)
            && m_serial->setParity (serialSettings()->m_parity) && m_serial->setStopBits (serialSettings()->m_stopBits)
            && m_serial->setFlowControl (serialSettings()->m_flowControl))
    {
        return true;
    }else
    {
        Q_EMIT portError(QString(tr("Can't configure the serial port: <b>%1</b>,\n"
                                    "error code: <b>%2</b>"))
                         .arg (serialSettings()->m_name).arg (m_serial->error ()));
        return false;
    }
}
/**
 * @brief Function to close serial port
 */
void Sender::closeSerialPort(void)
{
    if(m_serial != Q_NULLPTR){
        setPortOpen(false);
        m_serial->close();
        Q_EMIT portClosed(QString("Port closed: %1").arg(serialSettings()->m_name));
    }
}
/**
 * @brief Function to open serial port returns true when serial port
 *        was opened successfully otherwise false
 *
 * @return bool
 */
bool Sender::openSerialPort(void)
{
    bool result;

    m_serial->setPortName (serialSettings()->m_name);

    if(m_serial->open(QIODevice::ReadWrite))
    {
        result = checkPortSettings();

    }else
    {
        result = false;
    }
    return result;
}
