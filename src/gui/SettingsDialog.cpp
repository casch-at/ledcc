/*************************************************************
 * 	File Name       : settingsdialog.cpp
 *************************************************************
 *  Created on	 : Jan 30, 2013
 *  Author 		 : Christian Schwarzgruber
 *  Version		 : 1.0
 *************************************************************
 *  Description	 : SettingsDialog
 *************************************************************/

#include "SettingsDialog.hpp"
#include "ui_SettingsDialog.h"
#include "./aqp/alt_key.hpp"
#include "./aqp/aqp.hpp"

#include <QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QIntValidator>
#include <QLineEdit>
#include <QSettings>

#include  <QDebug>

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
SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    intValidator = new QIntValidator(0, 4000000, this);

    ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);

    connect(ui->applyButton, SIGNAL(clicked()),
            this, SLOT(apply()));
    connect(ui->portsBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(showPortInfo(int)));

    //        connect(ui->baudRateBox, SIGNAL(currentIndexChanged(int)),
    //                this, SLOT(checkCustomBaudRatePolicy(int)));

    fillPortsParameters();
    fillPortsInfo();
    updateSettings();
    restoreValues();
    AQP::accelerateWidget (this);  //Give each button a accelerater


}

/**
 * @brief
 *
 */
SettingsDialog::~SettingsDialog()
{
    delete ui;
}

/**
 * @brief
 *
 * @return SettingsDialog::SerialSettings
 */
SettingsDialog::SerialSettings SettingsDialog::settings() const
{
    return currentSettings;
}

/**
 * @brief
 *
 * @param idx
 */
void SettingsDialog::showPortInfo(int idx)
{
    if (idx != -1) {
        QStringList list = ui->portsBox->itemData(idx).toStringList();
        ui->descriptionLabel->setText(tr("Description: %1").arg(list.at(1)));
        ui->manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.at(2)));
        ui->locationLabel->setText(tr("Location: %1").arg(list.at(3)));
        ui->vidLabel->setText(tr("Vendor Identifier: %1").arg(list.at(4)));
        ui->pidLabel->setText(tr("Product Identifier: %1").arg(list.at(5)));
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
    ui->baudRateBox->addItem(QLatin1String("1200"), QSerialPort::Baud1200);
    ui->baudRateBox->addItem(QLatin1String("2400"), QSerialPort::Baud2400);
    ui->baudRateBox->addItem(QLatin1String("4800"), QSerialPort::Baud4800);
    ui->baudRateBox->addItem(QLatin1String("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QLatin1String("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QLatin1String("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QLatin1String("57600"), QSerialPort::Baud57600);
    ui->baudRateBox->addItem(QLatin1String("115200"), QSerialPort::Baud115200);
    ui->baudRateBox->setCurrentIndex (7);

    // fill data bits
    ui->dataBitsBox->addItem(QLatin1String("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QLatin1String("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QLatin1String("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QLatin1String("8"), QSerialPort::Data8);
    ui->dataBitsBox->setCurrentIndex(3);

    // fill parity
    ui->parityBox->addItem(QLatin1String("None"), QSerialPort::NoParity);
    ui->parityBox->addItem(QLatin1String("Even"), QSerialPort::EvenParity);
    ui->parityBox->addItem(QLatin1String("Odd"), QSerialPort::OddParity);
    ui->parityBox->addItem(QLatin1String("Mark"), QSerialPort::MarkParity);
    ui->parityBox->addItem(QLatin1String("Space"), QSerialPort::SpaceParity);

    // fill stop bits
    ui->stopBitsBox->addItem(QLatin1String("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsBox->addItem(QLatin1String("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox->addItem(QLatin1String("2"), QSerialPort::TwoStop);

    // fill flow control
    ui->flowControlBox->addItem(QLatin1String("None"), QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(QLatin1String("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(QLatin1String("XON/XOFF"), QSerialPort::SoftwareControl);
}

/**
 * @brief
 *
 */
void SettingsDialog::fillPortsInfo() // Fill the combobox with the available Ports
{
    QRegExp rx1( "ACM" );  // If ACM port is availably always set this port as current index
    ui->portsBox->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo:: availablePorts()) {
        QStringList list;

        //                ui->serialPortInfoListBox->addItem(list.first(), list);
        list << info.portName() << info.description()
             << info.manufacturer() << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString())
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : QString());

        ui->portsBox->addItem(list.first(), list);
        ui->portsBox->setCurrentIndex (0);
        for(int i=1; i<ui->portsBox->count (); i++){
            ui->portsBox->setCurrentIndex (i);
            if( ui->portsBox->currentText ().contains (rx1)){
                ui->portsBox->setCurrentIndex (i);
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
    currentSettings.name = ui->portsBox->currentText();

    currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    //            }
    currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

    // Data bits
    currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
    currentSettings.stringDataBits = ui->dataBitsBox->currentText();

    // Parity
    currentSettings.parity = static_cast<QSerialPort::Parity>(
                ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
    currentSettings.stringParity = ui->parityBox->currentText();

    // Stop bits
    currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
    currentSettings.stringStopBits = ui->stopBitsBox->currentText();

    // Flow control
    currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());
    currentSettings.stringFlowControl = ui->flowControlBox->currentText();
}

/**
 * @brief
 *
 */
void SettingsDialog::restoreValues() //Restore Settings of Serial Port
{

    QSettings settings("Settings","Kitchen Scale");

    currentSettings.baudRate =static_cast<QSerialPort::BaudRate>(settings.value (BaudeRate).toInt ());
    currentSettings.stringBaudRate = QString::number(settings.value (BaudeRate).toInt ());

    currentSettings.dataBits =  static_cast<QSerialPort::DataBits>(settings.value (DataBits).toInt ());
    currentSettings.stringDataBits =settings.value (DataBits).toString ();

    currentSettings.parity =  static_cast<QSerialPort::Parity>(settings.value (Parity).toInt ());
    currentSettings.stringParity = settings.value (Parity).toString ();

    currentSettings.stopBits = static_cast<QSerialPort::StopBits>(settings.value (StopBits).toInt ());
    currentSettings.stringStopBits = settings.value (StopBits).toString ();

    currentSettings.flowControl =  static_cast<QSerialPort::FlowControl>(settings.value (FlowControl).toInt ());
    currentSettings.stringFlowControl =settings.value (FlowControl).toString ();

}

/**
 * @brief
 *
 */
void SettingsDialog::saveValues() //Save Settings of Serial Port
{
    QSettings settings("Settings","Kitchen Scale");

    settings.setValue (BaudeRate,currentSettings.baudRate);
    settings.setValue (DataBits,currentSettings.dataBits);
    settings.setValue (Parity,currentSettings.parity);
    settings.setValue (StopBits,currentSettings.stopBits);
    settings.setValue (FlowControl,currentSettings.flowControl);
}

/**
 * @brief
 *
 */
void SettingsDialog::on_updateButton_clicked()
{
    fillPortsInfo ();
}
