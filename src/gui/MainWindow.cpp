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
#include "MainWindow.hpp"
#include "ui_MainWindow.h"
// Third Party
#include "alt_key.hpp"
#include "aqp.hpp"

// Application includes
#include "SettingsDialog.hpp"
#include "Sender.hpp"
#include "PortMessageBox.hpp"
#include "AnimationListWidget.hpp"
#include "AnimationPlayListWidget.hpp"
#include "AnimationHandler.hpp"
#include "Animations.hpp"
// Qt includes
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>
#include <QMenuBar>
#include <QToolBar>
#include <QByteArray>
#include <QEventLoop>
#include <QSerialPort>
#include <QShortcut>

#ifdef _DEBUG_
#include <QDebug>
#endif

// Namespaces
using namespace std;
/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :  //Init MainWindow
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_sdialog(new SettingsDialog),
    m_focusAnimationList(new  QShortcut(QKeySequence(tr("Alt+1")),this)),
    m_focusAnimationPlaylist(new  QShortcut(QKeySequence(tr("Alt+2")),this)),
    m_scSellectAll(new  QShortcut(QKeySequence(tr("Ctrl+A")),this)),
    m_createThread(new QThread)
{
    ui->setupUi(this); // Ui must be first created befor accessing the elements
    Animations *animations = new animations::Animations;
    ui->m_animationList->insertAnimationItems(animations->animationItemDefaultList());
    m_animationHandler = new animations::AnimationHandler(animations);
    setupSenderThread();
    readSettings ();
    connectSignals();
    ui->splitter->setStretchFactor(1,2);
    ui->m_animationList->setFocus();
    AQP::accelerateWidget (this);  //Give each button a accelerater

    Q_EMIT updateUi();
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow(void) //Deinit MainWindow
{

    delete m_sdialog;
    delete m_sender;
    delete ui;
}

/**
 * @brief MainWindow::closeEvent
 * @param event
 */
void MainWindow::closeEvent( QCloseEvent *event ) {
    if ( okToContinue() ) {
        Q_EMIT okClosePort();
        if(m_createThread->isRunning() || m_senderThread->isRunning())
            stopThreads();
        saveSettings ();
        m_sdialog->close();
        event->accept();
    }
    else
        event->ignore();
}

/**
 * @brief MainWindow::okToContinue
 * @return boolean
 */
bool MainWindow::okToContinue(void)
{ //Check if window is not modified
    if(isWindowModified ())
    {
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

/**
 * @brief MainWindow::resizeEvent
 * @param e
 */
void MainWindow::resizeEvent(QResizeEvent *e)
{
    QMainWindow::resizeEvent(e);
}

/**
 * @brief MainWindow::saveSettings
 */
void MainWindow::saveSettings(void){  //Save geometry of application
    QSettings settings("Schwarz Software Inc.","3D-LED Cube");
    settings.setValue (Settings::SMainWindowGeometrySettings,saveGeometry ());
}

/**
 * @brief MainWindow::readSettings
 */
void MainWindow::readSettings (void){ //Load geometry of application
    QSettings settings("Schwarz Software Inc.","3D-LED Cube");
    restoreGeometry (settings.value (Settings::SMainWindowGeometrySettings).toByteArray ());
}

/**
 * @author  Christian Schwarzgruber
 * @brief   MainWindow::updateUi gets called when the serial port gets opened or closed
 *          and when the animation playlist gets modified.
 *          Disables/Enables the playbutton and changes the Seriel connect button appropriated
 */
void MainWindow::updateUi(void)
{
    int inPlayList = ui->m_animationPlaylist->count();

    if(m_portOpened)
    {
        if(ui->m_openClosePortAction->text() == "Open port")
        {
            ui->m_openClosePortAction->setText(tr("Close port"));
            ui->m_openClosePortAction->setIcon( QIcon( "://images/disconnect.png"));
            ui->m_openClosePortAction->setToolTip(tr("Disconnect from seriell device  O"));
        }
        if( inPlayList && !ui->m_stopAction->isEnabled()){
            ui->m_playAction->setEnabled(true);
            ui->m_stopAction->setDisabled(true);
        }
        else{
            ui->m_playAction->setDisabled(true);
        }
    }else
    {
        if(ui->m_openClosePortAction->text() == "Close port")
        {
            ui->m_openClosePortAction->setText(tr("Open port"));
            ui->m_openClosePortAction->setIcon( QIcon( "://images/connect.png"));
            ui->m_openClosePortAction->setToolTip(tr("Connect to seriell device  O"));
        }
        ui->m_playAction->setDisabled(true);
        ui->m_stopAction->setDisabled(true);
    }

    if(inPlayList){
        ui->m_editAction->setEnabled(true);
        ui->m_clearAction->setEnabled(true);
        ui->m_removeAction->setEnabled(true);
        ui->m_moveDownAction->setEnabled(true);
        ui->m_moveUpAction->setEnabled(true);
        ui->m_duplicateAction->setEnabled(true);
        ui->m_clearAction->setEnabled(true);
    } else {
        ui->m_editAction->setDisabled(true);
        ui->m_clearAction->setDisabled(true);
        ui->m_removeAction->setDisabled(true);
        ui->m_moveDownAction->setDisabled(true);
        ui->m_moveUpAction->setDisabled(true);
        ui->m_duplicateAction->setDisabled(true);
        ui->m_clearAction->setDisabled(true);
        ui->m_clearAction->setDisabled(true);
    }
}

void MainWindow::portOpen(const QString &message)
{
    m_portOpened = true;
    ui->m_statusbar->showMessage(message,3000);
    updateUi();
}

void MainWindow::displayPortErrorMessage(const QString &message)
{
    PortMessageBox *msg = Q_NULLPTR;
    if(message.contains("#")){
        msg = new PortMessageBox(tr("Error"),message.split("#").first(),message.split("#").last(),this);
    }else{
        msg = new PortMessageBox(tr("Error"),message,this);
    }
    msg->exec();
    delete msg;
}

/**
 * @brief Gets called when user presses the port close Button and the port is open
 *
 * @param message which gets shown
 */
void MainWindow::closePort(const QString &message)
{
    PortMessageBox *msg = new PortMessageBox(tr("Close Port"),message,this);

    if(msg->exec() == QMessageBox::Ok)
        Q_EMIT okClosePort();

    delete msg;
}

/**
 * @brief Gets called when the serial port gets closed
 *
 * @param message which gets shown
 */
void MainWindow::portClosed(const QString &message)
{
    m_portOpened = false;
    ui->m_statusbar->showMessage(message,3000);
    stopThreads();
    updateUi();
}

/**
 * @brief Stop sender and animation Thread
 *
 */
void MainWindow::stopThreads()
{
//    m_currentAnimation->m_abort = true; //FIXME:: Is now in AnimationHandler
    m_sender->m_abort = true;
    ui->m_stopAction->setDisabled(true);
    m_stopPlay = false;
    m_createThread->quit(); // first quit threads befor wait
    m_senderThread->quit();

    m_senderThread->wait();
    m_createThread->wait();
//    m_currentAnimation->m_abort = false; //FIXME:: Is now in AnimationHandler
    m_sender->m_abort = false;
//    animationDone(); //FIXME:: Is now in AnimationHandler
}

/**
 * @brief Open or close serial port
 *
 */
void MainWindow::openCloseSerialPort(void)
{
    if(!m_senderThread->isRunning())
        m_senderThread->start();
    Q_EMIT openSerialInterface(m_sdialog->settings()); // call send thread
}

/**
 * @brief Setup sender Thread and create related connections
 *
 */
void MainWindow::setupSenderThread(void)
{
    m_portOpened = false;
    m_senderThread = new QThread;
    m_sender = new Sender;

    m_sender->moveToThread(m_senderThread); // move data send class to own thread
    m_senderThread->start(); // start sender thread, which is running for the entier live of the mainwindow

    connect(this,&MainWindow::openSerialInterface,m_sender,&Sender::openCloseSerialPort);
    connect(m_sender,&Sender::portOpened,this,&MainWindow::portOpen);
    connect(m_sender,&Sender::portClosed,this,&MainWindow::portClosed);
    connect(m_sender,&Sender::portError,this,&MainWindow::displayPortErrorMessage);
    connect(m_sender,&Sender::closePort,this,&MainWindow::closePort);
    connect(this,&MainWindow::okClosePort,m_sender,&Sender::closeSerialPort);
}

/**
 * @brief Create connections
 *
 */
void MainWindow::connectSignals(void)
{
    // Overall connections
    connect( ui->m_openClosePortAction, &QAction::triggered, this,&MainWindow::openCloseSerialPort);
    connect( ui->m_quitAction, &QAction::triggered, this,&MainWindow::close);
    connect( ui->m_aboutAction, &QAction::triggered,this,&MainWindow::about);
    connect( ui->m_settingsAction, &QAction::triggered,m_sdialog,&QWidget::show);
    connect( ui->m_stopAction, &QAction::triggered , this , &MainWindow::stopThreads);
    connect( ui->m_aboutQt, &QAction::triggered, this, &QApplication::aboutQt);

    // ListWidgets shortcuts
    connect( m_focusAnimationList, &QShortcut::activated, ui->m_animationList, &ListWidget::focus);
    connect( m_focusAnimationPlaylist, &QShortcut::activated, ui->m_animationPlaylist, &ListWidget::focus);
    connect( m_scSellectAll, &QShortcut::activated, ui->m_animationList, &ListWidget::selectAllItems);
    connect( m_scSellectAll, &QShortcut::activated, ui->m_animationPlaylist, &ListWidget::selectAllItems);

    // ListWidget interconnections
    connect( ui->m_animationList , &AnimationListWidget::addToPlaylist , ui->m_animationPlaylist , &AnimationPlayListWidget::newItem);
    connect( ui->m_animationList , &AnimationListWidget::showPropertiePreview , ui->animationPropertiesPreview , &AnimationPropertiesPreview::createPropertiePreview);
    connect( ui->m_animationPlaylist, &AnimationPlayListWidget::updateUi , this, &MainWindow::updateUi);
    //    connect( ui->m_animationPlaylist, &AnimationPlayListWidget::displayAnimationOptions, ui->animationAdjustGB, &AnimationOptions::displayAnimationOptions);
    connect( ui->m_animationPlaylist, &AnimationPlayListWidget::showPropertiePreview, ui->animationPropertiesPreview, &AnimationPropertiesPreview::createPropertiePreview);

    // Animation Playlist action
    connect( ui->m_clearAction, &QAction::triggered, ui->m_animationPlaylist, &AnimationPlayListWidget::clearList);
    connect( ui->m_duplicateAction, &QAction::triggered, ui->m_animationPlaylist, &AnimationPlayListWidget::duplicateItems);
    connect( ui->m_removeAction, &QAction::triggered, ui->m_animationPlaylist, &AnimationPlayListWidget::removeItems);
    connect( ui->m_moveUpAction, &QAction::triggered, ui->m_animationPlaylist, &AnimationPlayListWidget::moveItemsUpDown);
    connect( ui->m_moveDownAction, &QAction::triggered, ui->m_animationPlaylist, &AnimationPlayListWidget::moveItemsUpDown);
    connect( ui->m_editAction, &QAction::triggered, ui->m_animationPlaylist, &AnimationPlayListWidget::editItem);
    ui->m_animationPlaylist->addActions(QList<QAction*>()
                                        << ui->m_playAction << ui->m_stopAction << ui->m_editAction
                                        << ui->m_moveUpAction << ui->m_moveDownAction << ui->m_duplicateAction
                                        << ui->m_removeAction << ui->m_clearAction);
    //    ui->m_animationPlaylist->

}

/**
 * @brief About the Application
 *
 */
void MainWindow::about()
{
    QMessageBox::about(this, tr("About 3D-LED Cube"),
                       tr("<h2> 3D-LED Cube v0.1</h2>"
                          "<p> Copyright &copy; 2014 Christian Schwarzgruber"
                          "<p>The <b>3D-LED Cube</b> program was part of my thesis."
                          "This program lets you rearange the animation in the order you like it, you can even adjust speed,"
                          "delay, iterations and much more."));
}
