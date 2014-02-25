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

    QSerialPort *m_serial;
signals:

public Q_SLOTS:
    void sendAnimations(Draw::CubeArray &d);
    void stop();
private:
    bool m_stoped;
    bool m_running;

};

#endif // SENDTHREAD_HPP
