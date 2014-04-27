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
#include "SettingsDialog.hpp"
#include "ui_SettingsDialog.h"

/* Application includes */
#include "Config.hpp"

/* Qt includes */
#include <QCloseEvent>
#include <QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QIntValidator>
#include <QLineEdit>

#ifdef _DEBUG_
#include <QDebug>
#endif

SerialSettings* SerialSettings::m_instance = Q_NULLPTR;


/**
 * @brief
 *
 * @param parent
 */
//void SettingsDialog::closeEvent()
void SettingsDialog::closeEvent(QCloseEvent *e)
{
    hide();
//    e->accept();
}

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SettingsDialog)
{
    m_ui->setupUi(this);

    m_intValidator = new QIntValidator(0, 4000000, this);

    m_ui->m_baudRateBox->setInsertPolicy(QComboBox::NoInsert);
    m_ui->m_updateButton->setShortcut(QKeySequence::Refresh);
    connect(m_ui->m_applyButton, &QPushButton::clicked, this, &SettingsDialog::apply);
    void (QComboBox:: *signal)(int) = &QComboBox::currentIndexChanged;
    connect(m_ui->m_portsBox, signal, this, &SettingsDialog::showPortInfo);
    fillPortsParameters();
    fillPortsInfo();
    updateSettings();
    restoreValues();
}

/**
 * @brief
 *
 */
SettingsDialog::~SettingsDialog()
{
    delete m_ui;
}


/**
 * @brief
 *
 * @param idx
 */
void SettingsDialog::showPortInfo(int idx)
{
    if (idx != -1) {
        QStringList list = m_ui->m_portsBox->itemData(idx).toStringList();
        m_ui->m_descriptionL->setText(tr("Description: %1").arg(list.at(1)));
        m_ui->m_manufacturerL->setText(tr("Manufacturer: %1").arg(list.at(2)));
        m_ui->m_locationL->setText(tr("Location: %1").arg(list.at(3)));
        m_ui->m_vidL->setText(tr("Vendor Identifier: %1").arg(list.at(4)));
        m_ui->m_pidL->setText(tr("Product Identifier: %1").arg(list.at(5)));
    }
}

/**
 * @brief
 *
 */
void SettingsDialog::apply()
{
    updateSettings();
    saveValues();
    hide();
}

//void SettingsDialog::checkCustomBaudRatePolicy(int idx)
//{
//    bool isCustomBaudRate = !ui->m_baudRateBox->itemData(idx).isValid();
//    if (isCustomBaudRate) {
//        ui->m_baudRateBox->clearEditText();
//        QLineEdit *edit = ui->m_baudRateBox->lineEdit();
//        edit->setValidator(intValidator);
//    }
//    ui->m_baudRateBox->setEditable(isCustomBaudRate);
//}

/**
 * @brief
 *
 */
void SettingsDialog::fillPortsParameters()
{
    // fill baud rate (is not the entire list of available values,
    // desired values??, add your independently)
    m_ui->m_baudRateBox->addItem(QLatin1String("1200"), QSerialPort::Baud1200);
    m_ui->m_baudRateBox->addItem(QLatin1String("2400"), QSerialPort::Baud2400);
    m_ui->m_baudRateBox->addItem(QLatin1String("4800"), QSerialPort::Baud4800);
    m_ui->m_baudRateBox->addItem(QLatin1String("9600"), QSerialPort::Baud9600);
    m_ui->m_baudRateBox->addItem(QLatin1String("19200"), QSerialPort::Baud19200);
    m_ui->m_baudRateBox->addItem(QLatin1String("38400"), QSerialPort::Baud38400);
    m_ui->m_baudRateBox->addItem(QLatin1String("57600"), QSerialPort::Baud57600);
    m_ui->m_baudRateBox->addItem(QLatin1String("115200"), QSerialPort::Baud115200);
    m_ui->m_baudRateBox->setCurrentIndex (m_ui->m_baudRateBox->count());

    // fill data bits
    m_ui->m_dataBitsBox->addItem(QLatin1String("5"), QSerialPort::Data5);
    m_ui->m_dataBitsBox->addItem(QLatin1String("6"), QSerialPort::Data6);
    m_ui->m_dataBitsBox->addItem(QLatin1String("7"), QSerialPort::Data7);
    m_ui->m_dataBitsBox->addItem(QLatin1String("8"), QSerialPort::Data8);
    m_ui->m_dataBitsBox->setCurrentIndex(m_ui->m_dataBitsBox->count());

    // fill parity
    m_ui->m_parityBox->addItem(QLatin1String("None"), QSerialPort::NoParity);
    m_ui->m_parityBox->addItem(QLatin1String("Even"), QSerialPort::EvenParity);
    m_ui->m_parityBox->addItem(QLatin1String("Odd"), QSerialPort::OddParity);
    m_ui->m_parityBox->addItem(QLatin1String("Mark"), QSerialPort::MarkParity);
    m_ui->m_parityBox->addItem(QLatin1String("Space"), QSerialPort::SpaceParity);

    // fill stop bits
    m_ui->m_stopBitsBox->addItem(QLatin1String("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->m_stopBitsBox->addItem(QLatin1String("1.5"), QSerialPort::OneAndHalfStop);
#endif
    m_ui->m_stopBitsBox->addItem(QLatin1String("2"), QSerialPort::TwoStop);

    // fill flow control
    m_ui->m_flowControlBox->addItem(QLatin1String("None"), QSerialPort::NoFlowControl);
    m_ui->m_flowControlBox->addItem(QLatin1String("RTS/CTS"), QSerialPort::HardwareControl);
    m_ui->m_flowControlBox->addItem(QLatin1String("XON/XOFF"), QSerialPort::SoftwareControl);
}

/**
 * @brief Fill the combobox with the available Ports
 *
 */
void SettingsDialog::fillPortsInfo() //
{
    QRegExp rx1( "ACM" );  // If ACM port is availably always set this port as current index
    m_ui->m_portsBox->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        list << info.portName() << info.description()
             << info.manufacturer() << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString())
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : QString());

        m_ui->m_portsBox->addItem(list.first(), list);
        m_ui->m_portsBox->setCurrentIndex (0);
        for(int i=1; i<m_ui->m_portsBox->count (); i++){
            m_ui->m_portsBox->setCurrentIndex (i);
            if( m_ui->m_portsBox->currentText ().contains (rx1)){
                m_ui->m_portsBox->setCurrentIndex (i);
                break;
            }
        }
    }
}

/**
 * @brief
 *
 */
void SettingsDialog::updateSettings()  // Store the current settings in struct
{
    serialSettings()->m_name = m_ui->m_portsBox->currentText();

    serialSettings()->m_baudRate = static_cast<QSerialPort::BaudRate>(
                m_ui->m_baudRateBox->itemData(m_ui->m_baudRateBox->currentIndex()).toInt());
    //            }
    serialSettings()->m_stringBaudRate = QString::number(serialSettings()->m_baudRate);

    // Data bits
    serialSettings()->m_dataBits = static_cast<QSerialPort::DataBits>(
                m_ui->m_dataBitsBox->itemData(m_ui->m_dataBitsBox->currentIndex()).toInt());
    serialSettings()->m_stringDataBits = m_ui->m_dataBitsBox->currentText();

    // Parity
    serialSettings()->m_parity = static_cast<QSerialPort::Parity>(
                m_ui->m_parityBox->itemData(m_ui->m_parityBox->currentIndex()).toInt());
    serialSettings()->m_stringParity = m_ui->m_parityBox->currentText();

    // Stop bits
    serialSettings()->m_stopBits = static_cast<QSerialPort::StopBits>(
                m_ui->m_stopBitsBox->itemData(m_ui->m_stopBitsBox->currentIndex()).toInt());
    serialSettings()->m_stringStopBits = m_ui->m_stopBitsBox->currentText();

    // Flow control
    serialSettings()->m_flowControl = static_cast<QSerialPort::FlowControl>(
                m_ui->m_flowControlBox->itemData(m_ui->m_flowControlBox->currentIndex()).toInt());
    serialSettings()->m_stringFlowControl = m_ui->m_flowControlBox->currentText();
}

/**
 * @brief
 *
 */
void SettingsDialog::restoreValues() //Restore Settings of Serial Port
{
    serialSettings()->m_baudRate =static_cast<QSerialPort::BaudRate>(config()->get(Settings::SettingsBaudeRate).toInt ());
    serialSettings()->m_stringBaudRate = QString::number(config()->get(Settings::SettingsBaudeRate).toInt ());

    serialSettings()->m_dataBits =  static_cast<QSerialPort::DataBits>(config()->get(Settings::SettingsDataBits).toInt ());
    serialSettings()->m_stringDataBits = config()->get(Settings::SettingsDataBits).toString ();

    serialSettings()->m_parity =  static_cast<QSerialPort::Parity>(config()->get(Settings::SettingsParity).toInt ());
    serialSettings()->m_stringParity = config()->get(Settings::SettingsParity).toString ();

    serialSettings()->m_stopBits = static_cast<QSerialPort::StopBits>(config()->get(Settings::SettingsStopBits).toInt ());
    serialSettings()->m_stringStopBits = config()->get(Settings::SettingsStopBits).toString ();

    serialSettings()->m_flowControl =  static_cast<QSerialPort::FlowControl>(config()->get(Settings::SettingsFlowControl).toInt ());
    serialSettings()->m_stringFlowControl = config()->get(Settings::SettingsFlowControl).toString ();

    m_ui->m_baudRateBox->setCurrentText(serialSettings()->m_stringBaudRate);
    m_ui->m_dataBitsBox->setCurrentText(serialSettings()->m_stringDataBits);
    m_ui->m_parityBox->setCurrentText(serialSettings()->m_stringParity);
    m_ui->m_stopBitsBox->setCurrentText(serialSettings()->m_stringStopBits);
    m_ui->m_flowControlBox->setCurrentText(serialSettings()->m_stringFlowControl);
}

/**
 * @brief
 *
 */
void SettingsDialog::saveValues() //Save Settings of Serial Port
{
    config()->set(Settings::SettingsBaudeRate,serialSettings()->m_baudRate);
    config()->set(Settings::SettingsDataBits,serialSettings()->m_dataBits);
    config()->set(Settings::SettingsParity,serialSettings()->m_parity);
    config()->set(Settings::SettingsStopBits,serialSettings()->m_stopBits);
    config()->set(Settings::SettingsFlowControl,serialSettings()->m_flowControl);
}

/**
 * @brief
 *
 */
void SettingsDialog::on_m_updateButton_clicked()
{
    fillPortsInfo ();
}


SerialSettings *SerialSettings::instance()
{
    if(!m_instance)
        m_instance = new SerialSettings;
    return m_instance;
}
