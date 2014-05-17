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
class SerialSettings;

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    SerialSettings getSerialSettings();
protected:
    void closeEvent(QCloseEvent *e);
    void changeEvent(QEvent *e);
private Q_SLOTS:
    void showPortInfo(int idx);
    void apply();
Q_SIGNALS:
//    void hideDialog();
private:
    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();
    void restoreValues();
    void saveValues();
    Ui::SettingsDialog *m_ui;
    QIntValidator *m_intValidator;
    Q_DISABLE_COPY(SettingsDialog)
};

class SerialSettings
{

public:
    static SerialSettings *instance();
    QString m_name;
    qint32 m_baudRate;
    QString m_stringBaudRate;
    QSerialPort::DataBits m_dataBits;
    QString m_stringDataBits;
    QSerialPort::Parity m_parity;
    QString m_stringParity;
    QSerialPort::StopBits m_stopBits;
    QString m_stringStopBits;
    QSerialPort::FlowControl m_flowControl;
    QString m_stringFlowControl;
    ~SerialSettings(){}
private:
    SerialSettings(){}

    static SerialSettings *m_instance;
};

inline SerialSettings* serialSettings(){
    return SerialSettings::instance();
}

#endif // SETTINGSDIALOG_H
