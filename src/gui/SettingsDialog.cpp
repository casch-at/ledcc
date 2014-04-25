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
#include "alt_key.hpp"
#include "aqp.hpp"
#include <QCloseEvent>
#include <QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QIntValidator>
#include <QLineEdit>
#include <QSettings>
#ifdef _DEBUG_
#include <QDebug>
#endif
SerialSettings* SerialSettings::m_instance = Q_NULLPTR;
namespace{
    const QString BaudeRate("baude");
    const QString Parity("parity");
    const QString DataBits("databits");
    const QString StopBits("stopbits");
    const QString FlowControl("flowcontrol");
    }

/**
 * @brief
 *
 * @param parent
 */
//void SettingsDialog::closeEvent()
void SettingsDialog::closeEvent(QCloseEvent *e)
{
    e->accept();
}

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SettingsDialog)
{
    m_ui->setupUi(this);

    m_intValidator = new QIntValidator(0, 4000000, this);

    m_ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);
    m_ui->updateButton->setShortcut(QKeySequence::Refresh);
    connect(m_ui->applyButton, &QPushButton::clicked, this, &SettingsDialog::apply);
//    connect(m_ui->portsBox, &QComboBox::currentIndexChanged, this, &SettingsDialog::showPortInfo);
    fillPortsParameters();
    fillPortsInfo();
    updateSettings();
    restoreValues();
//    qRegisterMetaType<SettingsDialog::SerialSettings>("SettingsDialog::SerialSettings");
    AQP::accelerateWidget (this);  //Give each button a accelerater
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
        QStringList list = m_ui->portsBox->itemData(idx).toStringList();
        m_ui->descriptionLabel->setText(tr("Description: %1").arg(list.at(1)));
        m_ui->manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.at(2)));
        m_ui->locationLabel->setText(tr("Location: %1").arg(list.at(3)));
        m_ui->vidLabel->setText(tr("Vendor Identifier: %1").arg(list.at(4)));
        m_ui->pidLabel->setText(tr("Product Identifier: %1").arg(list.at(5)));
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
//    {
//        bool isCustomBaudRate = !ui->baudRateBox->itemData(idx).isValid();
//        if (isCustomBaudRate) {
//                ui->baudRateBox->clearEditText();
//                QLineEdit *edit = ui->baudRateBox->lineEdit();
//                edit->setValidator(intValidator);
//            }
//        ui->baudRateBox->setEditable(isCustomBaudRate);
//    }

/**
 * @brief
 *
 */
void SettingsDialog::fillPortsParameters()
{
    // fill baud rate (is not the entire list of available values,
    // desired values??, add your independently)
    m_ui->baudRateBox->addItem(QLatin1String("1200"), QSerialPort::Baud1200);
    m_ui->baudRateBox->addItem(QLatin1String("2400"), QSerialPort::Baud2400);
    m_ui->baudRateBox->addItem(QLatin1String("4800"), QSerialPort::Baud4800);
    m_ui->baudRateBox->addItem(QLatin1String("9600"), QSerialPort::Baud9600);
    m_ui->baudRateBox->addItem(QLatin1String("19200"), QSerialPort::Baud19200);
    m_ui->baudRateBox->addItem(QLatin1String("38400"), QSerialPort::Baud38400);
    m_ui->baudRateBox->addItem(QLatin1String("57600"), QSerialPort::Baud57600);
    m_ui->baudRateBox->addItem(QLatin1String("115200"), QSerialPort::Baud115200);
    m_ui->baudRateBox->setCurrentIndex (7);

    // fill data bits
    m_ui->dataBitsBox->addItem(QLatin1String("5"), QSerialPort::Data5);
    m_ui->dataBitsBox->addItem(QLatin1String("6"), QSerialPort::Data6);
    m_ui->dataBitsBox->addItem(QLatin1String("7"), QSerialPort::Data7);
    m_ui->dataBitsBox->addItem(QLatin1String("8"), QSerialPort::Data8);
    m_ui->dataBitsBox->setCurrentIndex(3);

    // fill parity
    m_ui->parityBox->addItem(QLatin1String("None"), QSerialPort::NoParity);
    m_ui->parityBox->addItem(QLatin1String("Even"), QSerialPort::EvenParity);
    m_ui->parityBox->addItem(QLatin1String("Odd"), QSerialPort::OddParity);
    m_ui->parityBox->addItem(QLatin1String("Mark"), QSerialPort::MarkParity);
    m_ui->parityBox->addItem(QLatin1String("Space"), QSerialPort::SpaceParity);

    // fill stop bits
    m_ui->stopBitsBox->addItem(QLatin1String("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsBox->addItem(QLatin1String("1.5"), QSerialPort::OneAndHalfStop);
#endif
    m_ui->stopBitsBox->addItem(QLatin1String("2"), QSerialPort::TwoStop);

    // fill flow control
    m_ui->flowControlBox->addItem(QLatin1String("None"), QSerialPort::NoFlowControl);
    m_ui->flowControlBox->addItem(QLatin1String("RTS/CTS"), QSerialPort::HardwareControl);
    m_ui->flowControlBox->addItem(QLatin1String("XON/XOFF"), QSerialPort::SoftwareControl);
}

/**
 * @brief
 *
 */
void SettingsDialog::fillPortsInfo() // Fill the combobox with the available Ports
{
    QRegExp rx1( "ACM" );  // If ACM port is availably always set this port as current index
    m_ui->portsBox->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo:: availablePorts()) {
        QStringList list;

        //                ui->serialPortInfoListBox->addItem(list.first(), list);
        list << info.portName() << info.description()
             << info.manufacturer() << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString())
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : QString());

        m_ui->portsBox->addItem(list.first(), list);
        m_ui->portsBox->setCurrentIndex (0);
        for(int i=1; i<m_ui->portsBox->count (); i++){
            m_ui->portsBox->setCurrentIndex (i);
            if( m_ui->portsBox->currentText ().contains (rx1)){
                m_ui->portsBox->setCurrentIndex (i);
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
    serialSettings()->m_name = m_ui->portsBox->currentText();

    serialSettings()->m_baudRate = static_cast<QSerialPort::BaudRate>(
                m_ui->baudRateBox->itemData(m_ui->baudRateBox->currentIndex()).toInt());
    //            }
    serialSettings()->m_stringBaudRate = QString::number(serialSettings()->m_baudRate);

    // Data bits
    serialSettings()->m_dataBits = static_cast<QSerialPort::DataBits>(
                m_ui->dataBitsBox->itemData(m_ui->dataBitsBox->currentIndex()).toInt());
    serialSettings()->m_stringDataBits = m_ui->dataBitsBox->currentText();

    // Parity
    serialSettings()->m_parity = static_cast<QSerialPort::Parity>(
                m_ui->parityBox->itemData(m_ui->parityBox->currentIndex()).toInt());
    serialSettings()->m_stringParity = m_ui->parityBox->currentText();

    // Stop bits
    serialSettings()->m_stopBits = static_cast<QSerialPort::StopBits>(
                m_ui->stopBitsBox->itemData(m_ui->stopBitsBox->currentIndex()).toInt());
    serialSettings()->m_stringStopBits = m_ui->stopBitsBox->currentText();

    // Flow control
    serialSettings()->m_flowControl = static_cast<QSerialPort::FlowControl>(
                m_ui->flowControlBox->itemData(m_ui->flowControlBox->currentIndex()).toInt());
    serialSettings()->m_stringFlowControl = m_ui->flowControlBox->currentText();
}

/**
 * @brief
 *
 */
void SettingsDialog::restoreValues() //Restore Settings of Serial Port
{

    QSettings settings("Settings","Kitchen Scale");

    serialSettings()->m_baudRate =static_cast<QSerialPort::BaudRate>(settings.value (BaudeRate).toInt ());
    serialSettings()->m_stringBaudRate = QString::number(settings.value (BaudeRate).toInt ());

    serialSettings()->m_dataBits =  static_cast<QSerialPort::DataBits>(settings.value (DataBits).toInt ());
    serialSettings()->m_stringDataBits =settings.value (DataBits).toString ();

    serialSettings()->m_parity =  static_cast<QSerialPort::Parity>(settings.value (Parity).toInt ());
    serialSettings()->m_stringParity = settings.value (Parity).toString ();

    serialSettings()->m_stopBits = static_cast<QSerialPort::StopBits>(settings.value (StopBits).toInt ());
    serialSettings()->m_stringStopBits = settings.value (StopBits).toString ();

    serialSettings()->m_flowControl =  static_cast<QSerialPort::FlowControl>(settings.value (FlowControl).toInt ());
    serialSettings()->m_stringFlowControl =settings.value (FlowControl).toString ();

}

/**
 * @brief
 *
 */
void SettingsDialog::saveValues() //Save Settings of Serial Port
{
    QSettings settings("Settings","Kitchen Scale");

    settings.setValue (BaudeRate,serialSettings()->m_baudRate);
    settings.setValue (DataBits,serialSettings()->m_dataBits);
    settings.setValue (Parity,serialSettings()->m_parity);
    settings.setValue (StopBits,serialSettings()->m_stopBits);
    settings.setValue (FlowControl,serialSettings()->m_flowControl);
}

/**
 * @brief
 *
 */
void SettingsDialog::on_updateButton_clicked()
{
    fillPortsInfo ();
}


SerialSettings *SerialSettings::instance()
{
    if(!m_instance)
        m_instance = new SerialSettings;
    return m_instance;
}
