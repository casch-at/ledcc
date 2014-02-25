#include "SendThread.hpp"
#include <QSerialPort>
#include <QDebug>

SendThread::SendThread(QObject *parent) :
    QObject(parent),
    m_stoped(false),
    m_running(false),
    m_serial(new QSerialPort)
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


void SendThread::openCloseSerialPort(const SettingsDialog::SerialSettings &s)  // Open the Serial port
{
    m_port = s;
    if(!m_serial->isOpen()){ // Get the status of the Serial port
        bool result = openSerialPort();
        if(result)
        {
//            ui->statusbar->showMessage (tr("Connected to %1 : %2, %3, %4, %5, %6")
//                                        .arg (m_port.name).arg (m_port.stringBaudRate)
//                                        .arg (m_port.stringDataBits).arg (m_port.stringParity)
//                                        .arg (m_port.stringStopBits).arg (m_port.stringFlowControl));
        }else
        {
            m_serial->close();
//            QMessageBox::warning (this,tr("Error"),
//                                  tr("Can't open serial port: %1 - error code: %2\n\n\n"
//                                     "Check if device is connected properly!")
//                                  .arg (m_port.name).arg (m_serial->error ()));
//            ui->statusbar->showMessage(tr("Open error"),3000);
        }
    }
    else{
//        int flag = QMessageBox::information (this,tr("Closing port")
//                                             ,tr("Do you really want close the serial port?\n %1")
//                                             .arg(m_port.name),QMessageBox::Ok,QMessageBox::Cancel);
//        if(flag == QMessageBox::Ok)
//            closeSerialPort();

    }
//    updateUi ();
}
/**
 * @brief MainWindow::checkPortSettings
 * @return
 */
bool SendThread::checkPortSettings(void)
{
    if( m_serial->setBaudRate (m_port.baudRate) && m_serial->setDataBits (m_port.dataBits)
            && m_serial->setParity (m_port.parity) && m_serial->setStopBits (m_port.stopBits)
            && m_serial->setFlowControl (m_port.flowControl))
    {
        return true;
    }else
    {
//        QMessageBox::critical (this,tr("Error"),
//                               tr("Can't configure the serial port: %1,\n"
//                                  "error code: %2")
//                               .arg (m_port.name).arg (m_serial->error ()));
        return false;
    }
}
/**
 * @brief MainWindow::closeSerialPort
 */
void SendThread::closeSerialPort(void)
{
    m_serial->close();
//    ui->statusbar->showMessage(tr("Port closed: %1").arg (m_port.name),3000);
}
/**
 * @brief MainWindow::openSerialPort
 * @return
 */
bool SendThread::openSerialPort(void)
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
