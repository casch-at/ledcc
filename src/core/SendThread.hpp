#ifndef SENDTHREAD_HPP
#define SENDTHREAD_HPP
#include <QThread>
#include <Global.hpp>
#include "animations/Draw.hpp"
#include "SettingsDialog.hpp"
class QSerialPort;

class SendThread : public QObject
{
    Q_OBJECT
public:
    explicit SendThread(QObject *parent = Q_NULLPTR);

    bool isRunning(void){
        return m_running;
    }

Q_SIGNALS:

public Q_SLOTS:
    void sendAnimations(Draw::CubeArray &d);
    void stop();
    void openCloseSerialPort(const SettingsDialog::SerialSettings &s);

private:
    bool m_stoped;
    bool m_running;
    QSerialPort *m_serial;
private:
    bool openSerialPort();
    void closeSerialPort();
    bool checkPortSettings();
    SettingsDialog::SerialSettings m_port;
};

#endif // SENDTHREAD_HPP
