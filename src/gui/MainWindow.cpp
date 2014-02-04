/*************************************************************
 * 	File Name       : mainwindows.cpp
 *************************************************************
 *  Created on	 : Jan 30, 2013
 *  Author 		 : Christian Schwarzgruber
 *  Version		 : 1.0
 *************************************************************
 *  Description	 : MainWindow
 *************************************************************/

#include "MainWindow.hpp"
#include "ui_MainWindow.h"
//#include "adddialog.hpp"
#include "SettingsDialog.hpp"
#include "./aqp/alt_key.hpp"
#include "./aqp/aqp.hpp"
#include "DebugDockWidget.hpp"
#include "AnimationThread.hpp"
#include "Animations.hpp"
#include "SendThread.hpp"
#include "Global.hpp"


//#include <QListWidget>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QModelIndex>
#include <QCloseEvent>
#include <QFileDialog>
#include <QSettings>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QByteArray>
#include <QEventLoop>
#include <QSerialPort>
#include <QMultiMap>
#include <iostream>

//#ifdef _DEBUG_
#include <QDebug>
//#endif
using namespace std;
#define ANIMATIONS 12
// TODO:: Write cleanUp

namespace{
    const QString GeometrySettings("geometry");

    }



//Q_DECLARE_METATYPE(AnimationStruct)
MainWindow::MainWindow(QWidget *parent) :  //Init MainWindow
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    sdialog(new SettingsDialog),
    playList(new QHash<QString,AnimationStruct>),
    alist(new QHash<QString,AnimationStruct>),
    serial(new QSerialPort)
{
    qRegisterMetaType<CubeArray>("CubeArray");
    qRegisterMetaType<AnimationStruct>("AnimationStruct");
    ui->setupUi(this);
#ifdef DEBUGWINDOW
    debugDockWidget = new DebugDockWidget(this);
    addDockWidget(Qt::RightDockWidgetArea,debugDockWidget,Qt::Vertical );
#endif
    readSettings ();
    createActions ();
    createToolbar ();
    connectSignals();
    setupAnimationList();
    playAction->setDisabled(true);
    pauseAction->setDisabled(true);
    //    qDebug() << "MainWindow parent: " << parent;
    //    qDebug() << "MainWindow aThread  parent: " << aThread->parent();
    AQP::accelerateWidget (this);  //Give each button a accelerater
}

MainWindow::~MainWindow(void) //Deinit MainWindow
{
    delete ui;
}

void MainWindow::closeEvent( QCloseEvent *event ) {  //Close application
    if ( okToContinue() ) {
        if(serial.isOpen())
            serial.close();
        saveSettings ();
        close ();
        sdialog->close();
        event->accept();
    }
    else
        event->ignore();
}

bool MainWindow::okToContinue(void) { //Check if window is not modified
    if(isWindowModified ()){
        int r = QMessageBox::warning( this, tr( "3D-LED Cube" ),
                                      tr( "Do you really want to exit the programm?" ),
                                      QMessageBox::Yes | QMessageBox::Default,
                                      QMessageBox::Cancel | QMessageBox::Escape );

        if ( r == QMessageBox::Yes )
            return true;
        else if ( r == QMessageBox::Cancel )
            return false;
        return true;
    }
    return true;
}

void MainWindow::resizeEvent(QResizeEvent *e){ //Resize Window
    QMainWindow::resizeEvent(e);
}

void MainWindow::saveSettings(void){  //Save geometry of application
    QSettings settings("Schwarz Software Inc.","3D-LED Cube");
    settings.setValue (GeometrySettings,saveGeometry ());
}

void MainWindow::readSettings (void){ //Load geometry of application
    QSettings settings("Schwarz Software Inc.","3D-LED Cube");
    restoreGeometry (settings.value (GeometrySettings).toByteArray ());
}

void MainWindow::clearToolButtonClicked()
{
    ui->animationPlaylistLW->clear();
    setWindowModified (false);
}

/**
 * @author  Christian Schwarzgruber
 * @brief   MainWindow::updateUi gets called when the serial port gets opened or closed
 *          and when the animation playlist gets modified.
 *          Disables/Enables the playbutton and changes the Seriel connect button appropriated
 */
void MainWindow::updateUi(void) // Update Button state
{
    if(serial.isOpen()){
        openPortAction->setText(tr("Close port O"));
        openPortAction->setIcon( QIcon( "://images/disconnect.png"));
        openPortAction->setToolTip(tr("Disconnect from seriell device  O"));
        if(ui->animationPlaylistLW->count())
            playAction->setEnabled(true);
        else
            playAction->setDisabled(true);
    }else
    {
        openPortAction->setText(tr("Open port O"));
        openPortAction->setIcon( QIcon( "://images/connect.png"));
        openPortAction->setToolTip(tr("Connect to seriell device  O"));
        playAction->setDisabled(true);
    }
}

void MainWindow::playAnimationFromList(void)
{
    static int row;
    int count = ui->animationPlaylistLW->count();
    if(count){
        if(row >= count)
            row=0;
        playAction->setDisabled(true);
        pauseAction->setEnabled(true);
        currentAnimation = alist->value(ui->animationPlaylistLW->item(row++)->text());
        emit startAnimation(currentAnimation);
    }else{
        playAction->setDisabled(true);
    }
}

void MainWindow::setupAnimationList()
{

    AnimationStruct a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13;
    a1.name = QString("Firework");
    a1.id = 1;
    a1.speed = 30;
    a1.particle = 20;
    a1.iteration = 10;
    alist->insert(a1.name,a1);
    a2.name = QString("Lift");
    a2.iteration = 5;
    a2.speed = 100;
    a2.delay = 200;
    alist->insert(a2.name,a2);
    a3.name = QString("Wall");
    a3.axis = Y_AXIS;
    a3.direction = FORWARED;
    a3.speed = 50;
    alist->insert(a3.name,a3);
    a4.name = QString("Rain");
    a4.speed = 50;
    a4.iteration = 20;
    alist->insert(a4.name,a4);
    a5.name = QString("Random Z-Axis Lift");
    a5.iteration = 10;
    a5.speed = 40;
    alist->insert(a5.name,a5);
    a6.name = QString("Wire Box Corner Shrink Grow");
    a6.iteration = 8;
    a6.speed = 50;
    alist->insert(a6.name,a6);
    a7.name = QString("Wire Box Center Shrink Grow");
    a7.speed = 50;
    a7.invert = true;
    alist->insert(a7.name,a7);
    a8.name = QString("Axis Nail Wall");
    a8.speed = 50;
    a8.axis = X_AXIS;
    a8.invert = true;
    alist->insert(a8.name,a8);
    a9.name= QString("Loadbar");
    a9.id = 1;
    a9.speed = 20;
    a9.axis = Z_AXIS;
    a9.iteration = 10;
    alist->insert(a9.name,a9);
    a10.name = QString("Random Spark Flash");
    a10.iteration = 5;
    a10.speed =50;
    a10.leds = 20;
    alist->insert(a10.name,a10);
    a11.name = QString("Random Spark");
    a11.leds = 50;
    a11.speed = 20;
    alist->insert(a11.name,a11);
    a12.name = QString("Random Filler");
    a12.speed = 40;
    a12.state = ON;
    alist->insert(a12.name,a12);
    a13.name = QString("String Fly");
    a13.text = QString("LED CUBE");
    a13.speed = 80;
    alist->insert(a13.name,a13);
    QHash<QString,AnimationStruct>::const_iterator i;
    for(i = alist->constBegin(); i!= alist->constEnd(); i++)
        ui->availableAnimationsLW->addItem(i.key());
}

void MainWindow::openCloseSerialPort(void)  // Open the Serial port
{
    m_port = sdialog->settings();

    if(!serial.isOpen()){ // Get the status of the Serial port
        bool result = openSerialPort();
        if(result)
        {
            ui->statusbar->showMessage (tr("Connected to %1 : %2, %3, %4, %5, %6")
                                        .arg (m_port.name).arg (m_port.stringBaudRate)
                                        .arg (m_port.stringDataBits).arg (m_port.stringParity)
                                        .arg (m_port.stringStopBits).arg (m_port.stringFlowControl));
        }else
        {
            serial.close();
            QMessageBox::warning (this,tr("Error"),
                                  tr("Can't open serial port: %1 - error code: %2\n\n\n"
                                     "Check if device is connected properly!")
                                  .arg (m_port.name).arg (serial.error ()));
            ui->statusbar->showMessage(tr("Open error"),3000);
        }
    }
    else{
        int flag = QMessageBox::information (this,tr("Closing port")
                                             ,tr("Do you really want close the serial port?\n %1")
                                             .arg(m_port.name),QMessageBox::Ok,QMessageBox::Cancel);
        if(flag == QMessageBox::Ok)
            closeSerialPort();

    }
    updateUi ();
}

bool MainWindow::checkPortSettings(void)
{
    if( serial.setBaudRate (m_port.baudRate) && serial.setDataBits (m_port.dataBits)
            && serial.setParity (m_port.parity) && serial.setStopBits (m_port.stopBits)
            && serial.setFlowControl (m_port.flowControl))
    {
        return true;
    }else
    {
        QMessageBox::critical (this,tr("Error"),
                               tr("Can't configure the serial port: %1,\n"
                                  "error code: %2")
                               .arg (m_port.name).arg (serial.error ()));
        return false;
    }
}

void MainWindow::closeSerialPort(void)
{
    serial.close();
    ui->statusbar->showMessage(tr("Port closed: %1").arg (m_port.name),3000);
}

bool MainWindow::openSerialPort(void)
{
    bool result;

    serial.setPortName (m_port.name);

    if(serial.open(QIODevice::ReadWrite))
    {
        result = checkPortSettings();

    }else
    {
        result = false;
    }
    return result;
}

void MainWindow::readData()
{
    //        setData (serial->readAll ());
#ifdef DEBUGWINDOW
    debugDockWidget->addLine("Received: " + QString(m_data) + "\n" );
#endif
}

#ifdef DEBUGWINDOW
void MainWindow::sendData()
{
    int i = debugDockWidget->sendString().toInt();
    debugDockWidget->addLine(QString("Sent: %1").arg(i));
    writeData(i);
}
void MainWindow::writeData(const char c) //Function to write data to serial port
{
    qDebug() << "Bytes to write: " + c;
    if(serial->putChar(c))
        qDebug() << "Send successfully";
}
#endif


void MainWindow::on_availableAnimationsLW_itemDoubleClicked(QListWidgetItem *item)
{
    ui->animationPlaylistLW->addItem(item->text());
    updateUi();
}

void MainWindow::on_animationPlaylistLW_itemDoubleClicked(QListWidgetItem *item)
{
    delete item;
    updateUi();
}

void MainWindow::connectSignals(void) //Connect Signals
{
    connect (openPortAction,&QAction::triggered,
             this,&MainWindow::openCloseSerialPort);
    connect(quitAction,&QAction::triggered,
            this,&MainWindow::close);
    connect (quitAction,&QAction::triggered,
             this,&MainWindow::clearToolButtonClicked);
    connect (aboutAction,&QAction::triggered,
             this,&MainWindow::about);
    connect(clearAction,&QAction::triggered,
            this,&MainWindow::clearToolButtonClicked);
    connect(settingAction,&QAction::triggered,
            sdialog,&QWidget::show);
#ifdef DEBUGWINDOW
    connect(debugDockWidget,&DebugDockWidget::sendStringChanged,
            this,&MainWindow::sendData);
#endif
}

void MainWindow::createActions(void)  // Creat action for the toolbar
{
    quitAction = new QAction( tr( "Quit Ctrl+C" ), this );
    quitAction->setIcon( QIcon( "://images/application-exit.png" ) );
    quitAction->setShortcut( QKeySequence::Quit );
    quitAction->setStatusTip( tr( "Exit application" ) );

    clearAction = new QAction( tr( "Clear F5" ), this );
    clearAction->setIcon( QIcon( "://images/clear.png" ) );
    clearAction->setShortcut( QKeySequence::Refresh );
    clearAction->setStatusTip( tr( "Clear item list" ) );

    aboutAction = new QAction( tr( "About Ctrl+A" ), this );
    aboutAction->setIcon( QIcon( "://images/help-about.png" ) );
    aboutAction->setShortcut(tr( "Ctrl+A" ));
    aboutAction->setStatusTip( tr( "About Kitchen Scale" ) );

    settingAction = new QAction( tr( "Settings Ctrl+I" ), this );
    settingAction->setIcon( QIcon( "://images/package_settings.png" ) );
    settingAction->setShortcut(tr( "Ctrl+I" ));
    settingAction->setStatusTip( tr( "Configure Kitchen Scale" ) );

    playAction = new QAction( tr("Play Animation R"),this);
    playAction->setIcon(QIcon("://images/media-playback-start-9.png"));
    playAction->setShortcut(tr("R"));
    playAction->setToolTip(tr("Play Animation R"));

    pauseAction = new QAction( tr("Pause Animations P"),this);
    pauseAction->setIcon(QIcon("://images/media-playback-pause-7.png"));
    pauseAction->setShortcut(tr("P"));
    pauseAction->setToolTip(tr("Pause Animations P"));

    openPortAction = new QAction( tr("Open port O"), this);
    openPortAction->setIcon( QIcon( "://images/connect.png"));
    openPortAction->setShortcut(tr("O"));
    openPortAction->setToolTip(tr("Connect to seriell device  O"));
}

void MainWindow::createToolbar()
{
    QSize size(32,32);

    mainToolBar = new QToolBar();

    mainToolBar->setObjectName ("Main Toolbar");
    mainToolBar->addAction(quitAction);
    mainToolBar->addSeparator();

    mainToolBar->addAction(openPortAction);
    mainToolBar->addSeparator();

    mainToolBar->addAction(playAction);
    mainToolBar->addAction(pauseAction);
    mainToolBar->addSeparator();

    mainToolBar->addAction (clearAction);
    mainToolBar->addSeparator ();

    mainToolBar->addAction(settingAction);

    mainToolBar->setIconSize (size);
    mainToolBar->setWindowTitle("Main Toolbar");
    this->addToolBar (mainToolBar);

    helpToolBar = new QToolBar( );
    helpToolBar->setObjectName ("Help Toolbar");
    helpToolBar->setLayoutDirection (Qt::RightToLeft);
    helpToolBar->addAction (aboutAction);
    helpToolBar->setIconSize (size);
    helpToolBar->setWindowTitle("Help Toolbar");
    this->addToolBar (Qt::TopToolBarArea,helpToolBar);
    this->helpToolBar->setObjectName ("Hello");
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About 3D-LED Cube"),
                       tr("<h2> 3D-LED Cube v0.1</h2>"
                          "<p> Copyright &copy; 2014 Christian Schwarzgruber"
                          "<p>The <b>3D-LED Cube</b> program was part of my thesis."
                          "This program lets you rearange the animation in the order you like it, you can even adjust speed,"
                          "delay, iterations and much more."));
}
