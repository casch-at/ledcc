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
    void portOpened(const QString &param);
    void portClosed(const QString &param);
    void portError(const QString &param);
    void closePort(const QString &param);
public Q_SLOTS:
    void sendAnimation(const Draw::CubeArray &d);
    void stop();
    void openCloseSerialPort(const SettingsDialog::SerialSettings &s);
    void closeSerialPort();

private:
    QSerialPort *m_serial;
    QString m_param;
    bool m_stoped;
    bool m_running;
    SettingsDialog::SerialSettings m_port;
private:
    bool openSerialPort();
    bool checkPortSettings();
};

#endif // SENDER_HPP
