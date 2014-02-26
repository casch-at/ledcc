#ifndef SENDER_HPP
#define SENDER_HPP
#include <QThread>
#include "Global.hpp"
#include "animations/Draw.hpp"
#include "SettingsDialog.hpp"

class QSerialPort;

class Sender : public QObject
{
    Q_OBJECT
public:
    explicit Sender(QObject *parent = Q_NULLPTR);

    bool isRunning(void){
        return m_running;
    }

Q_SIGNALS:
    void portStatus(QString param);
    void portError(QString param);
public Q_SLOTS:
    void sendAnimation(const Draw::CubeArray &d);
    void stop();
    void openCloseSerialPort(const SettingsDialog::SerialSettings &s);

private:
    QSerialPort *m_serial;
    QString m_param;
    bool m_stoped;
    bool m_running;
    SettingsDialog::SerialSettings m_port;
private:
    bool openSerialPort();
    void closeSerialPort();
    bool checkPortSettings();
};

#endif // SENDER_HPP
