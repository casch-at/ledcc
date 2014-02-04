/*************************************************************
 * 	File Name       : settingsdialog.hpp
 *************************************************************
 *  Created on	 : Jan 30, 2013
 *  Author 		 : Christian Schwarzgruber
 *  Version		 : 1.0
 *************************************************************
 *  Description	 : SettingsDialog Class
 *************************************************************/

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QToolButton>


namespace Ui {
    class SettingsDialog;
    }
/*Forward deceleration*/
class QIntValidator;
class QSettings;

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    struct SerialSettings {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
    };

    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    SerialSettings settings() const;

private slots:
    void showPortInfo(int idx);
    void apply();
    //    void checkCustomBaudRatePolicy(int idx);
    void on_updateButton_clicked();

private:
    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();
    void restoreValues();
    void saveValues();

private:
    Ui::SettingsDialog *ui;
    SerialSettings currentSettings;
    QIntValidator *intValidator;
};

#endif // SETTINGSDIALOG_H
