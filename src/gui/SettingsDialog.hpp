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
QT_BEGIN_NAMESPACE
class QIntValidator;
class QSettings;
QT_END_NAMESPACE

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
protected:
    virtual void closeEvent(QCloseEvent *);
public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    SerialSettings settings() const;

private Q_SLOTS:
    void showPortInfo(int idx);
    void apply();
    //    void checkCustomBaudRatePolicy(int idx);
    void on_updateButton_clicked();
//Q_SIGNALS:
//    void hideDialog();
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
    Q_DISABLE_COPY(SettingsDialog)
};
#endif // SETTINGSDIALOG_H
