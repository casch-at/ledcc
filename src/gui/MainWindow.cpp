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
#include "AnimationOptions.hpp"

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
using namespace animations;
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
    m_createThread(new QThread),
    m_animationList(new AnimationListWidget(this)),
    m_animationPlaylist(new AnimationPlayListWidget(this))
{
    ui->setupUi(this);
    setupSenderThread();
    readSettings ();
    createActions ();
    createToolbar ();
    setupAnimationItems();
    connectSignals();
    ui->splitter->setStretchFactor(1,2);
    ui->m_animationListLay->addWidget(m_animationList);
    ui->m_animationPlaylistLay->addWidget(m_animationPlaylist);
    m_animationList->setFocus();

    AQP::accelerateWidget (this);  //Give each button a accelerater

    Q_EMIT updateUi();
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow(void) //Deinit MainWindow
{

    delete m_createThread;
    delete m_senderThread;
    delete m_sdialog;
    delete m_sender;
    delete m_animationList;
    delete m_animationPlaylist;
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
    int inPlayList = m_animationPlaylist->count();

    if(m_portOpened)
    {
        if(m_openPortAction->text() == "Open port")
        {
            m_openPortAction->setText(tr("Close port"));
            m_openPortAction->setIcon( QIcon( "://images/disconnect.png"));
            m_openPortAction->setToolTip(tr("Disconnect from seriell device  O"));
        }
        if( inPlayList && !m_animationPlaylist->m_stopAction->isEnabled()){
            m_animationPlaylist->m_playAction->setEnabled(true);
            m_animationPlaylist->m_stopAction->setDisabled(true);
        }
        else{
            m_animationPlaylist->m_playAction->setDisabled(true);
        }
    }else
    {
        if(m_openPortAction->text() == "Close port")
        {
            m_openPortAction->setText(tr("Open port"));
            m_openPortAction->setIcon( QIcon( "://images/connect.png"));
            m_openPortAction->setToolTip(tr("Connect to seriell device  O"));
        }
        m_animationPlaylist->m_playAction->setDisabled(true);
        m_animationPlaylist->m_stopAction->setDisabled(true);
    }

    if(inPlayList){
        m_animationPlaylist->m_editAction->setEnabled(true);
        m_animationPlaylist->m_clearAction->setEnabled(true);
        m_animationPlaylist->m_removeAction->setEnabled(true);
        m_animationPlaylist->m_moveDownAction->setEnabled(true);
        m_animationPlaylist->m_moveUpAction->setEnabled(true);
        m_animationPlaylist->m_duplicateAction->setEnabled(true);
    } else {
        m_animationPlaylist->m_editAction->setDisabled(true);
        m_animationPlaylist->m_clearAction->setDisabled(true);
        m_animationPlaylist->m_removeAction->setDisabled(true);
        m_animationPlaylist->m_moveDownAction->setDisabled(true);
        m_animationPlaylist->m_moveUpAction->setDisabled(true);
        m_animationPlaylist->m_duplicateAction->setDisabled(true);
    }
}

/**
 * @author Christian Schwarzgruber
 * @brief MainWindow::playNextAnimation
 *
 * @param QString &a
 */
void MainWindow::playNextAnimation(const AnimationItem *item)
{
    if(item == Q_NULLPTR){
        stopThreads();
        return;
    }

    m_currentAnimation = m_animationHash.value(item->text());
    connect(m_createThread,&QThread::started,m_currentAnimation,&animations::Animation::createAnimation);
    connect(m_currentAnimation, &animations::Animation::done, m_createThread, &QThread::quit);
    //    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    //    connect(createThread , &QThread::finished, createThread, &QThread::deleteLater);
    connect(m_currentAnimation,&animations::Animation::done,this,&MainWindow::animationDone);

    updateAnimation(item);
    m_createThread->start();
}

void MainWindow::playAnimations()
{
    m_stopPlay = true;

    if(!m_senderThread->isRunning())
        m_senderThread->start();

    m_animationPlaylist->m_playAction->setDisabled(true);
    m_animationPlaylist->m_stopAction->setEnabled(true);
    playNextAnimation(m_animationPlaylist->getNextAnimation());
}


void MainWindow::animationDone()
{
    disconnect(m_createThread,&QThread::started,m_currentAnimation,&Animation::createAnimation);
    disconnect(m_currentAnimation, &Animation::done, m_createThread, &QThread::quit);
    disconnect(m_currentAnimation,&Animation::done,this,&MainWindow::animationDone);

    m_createThread->wait();

    if(m_stopPlay)
        playNextAnimation(m_animationPlaylist->getNextAnimation());
    else
        updateUi();

}

void MainWindow::updateItemToolTip(const Options &aOptions)
{
    QList<QListWidgetItem*> items = m_animationPlaylist->selectedItems();
    if(!items.isEmpty())
    {
        AnimationItem *item = dynamic_cast<AnimationItem*>(items.first());
        item->setOptions(const_cast<Options&>(aOptions));

        m_animationHash.value(item->text())->createAnimationTooltipAsRichText(item);
        ui->animationPropertiesPreview->createPropertiePreview(
                    m_animationHash.value( item->text() )->getAnimationPropertiesAsPlainText( item ) );
        if(m_currentAnimation->getName().compare(item->text()) == 0 /*&& createThread->isRunning()*/)
        {
            updateAnimation(item);
        }
    }
}


void MainWindow::portOpen(const QString &message)
{
    m_portOpened = true;
    ui->statusbar->showMessage(message,3000);
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
    ui->statusbar->showMessage(message,3000);
    stopThreads();
    updateUi();
}

/**
 * @brief Stop sender and animation Thread
 *
 */
void MainWindow::stopThreads()
{
    m_currentAnimation->m_abort = true;
    m_sender->m_abort = true;
    m_animationPlaylist->m_stopAction->setDisabled(true);
    m_stopPlay = false;
    m_createThread->quit(); // first quit threads befor wait
    m_senderThread->quit();

    m_senderThread->wait();
    m_createThread->wait();
    m_currentAnimation->m_abort = false;
    m_sender->m_abort = false;
    animationDone();
}


/**
 * @brief     QStringList list;
    list.append("Hello 123");
    list.append("You 12.3");
    ui->dockWidget->createPropertiePreview(dynamic_cast<AxisNailWall*>(animation.value(ANIMATIONS::AxisNailWall))->getAnimationProperties());
 *
 * @param item
 */
void MainWindow::showPropertiesPreview(QListWidgetItem *item)
{
    ui->animationPropertiesPreview->createPropertiePreview(
                m_animationHash.value( item->text() )->getAnimationPropertiesAsPlainText(
                    dynamic_cast<AnimationItem*>(item) ) );
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
    // Action connections
    connect( m_openPortAction, &QAction::triggered, this,&MainWindow::openCloseSerialPort);
    connect( m_quitAction, &QAction::triggered, this,&MainWindow::close);
    connect( m_aboutAction, &QAction::triggered,this,&MainWindow::about);
    connect( m_settingAction, &QAction::triggered,m_sdialog,&QWidget::show);
    connect( m_animationPlaylist->m_playAction, &QAction::triggered , this , &MainWindow::playAnimations);
    connect( m_animationPlaylist->m_stopAction, &QAction::triggered , this , &MainWindow::stopThreads);
    connect( m_aboutQt, &QAction::triggered, this, &QApplication::aboutQt);
    // Animation properties ready connection
    //    connect( ui->animationAdjustGB , &AnimationOptions::optionsReady , this, &MainWindow::updateItemToolTip);

    // ListWidget shortcut sellect all connections
    connect( m_focusAnimationList, &QShortcut::activated, m_animationList, &ListWidget::focus);
    connect( m_focusAnimationPlaylist, &QShortcut::activated, m_animationPlaylist, &ListWidget::focus);
    connect( m_scSellectAll, &QShortcut::activated, m_animationList, &ListWidget::selectAllItems);
    connect( m_scSellectAll, &QShortcut::activated, m_animationPlaylist, &ListWidget::selectAllItems);

    // ListWidget interconnections
    connect( m_animationList , &AnimationListWidget::addToPlaylist , m_animationPlaylist , &AnimationPlayListWidget::newItem);
    connect( m_animationList , &ListWidget::showPropertiePreview , this , &MainWindow::showPropertiesPreview);
    connect( m_animationPlaylist, &AnimationPlayListWidget::updateUi , this, &MainWindow::updateUi);
    //    connect( m_animationPlaylist, &AnimationPlayListWidget::displayAnimationOptions, ui->animationAdjustGB, &AnimationOptions::displayAnimationOptions);
    connect( m_animationPlaylist, &ListWidget::showPropertiePreview, this, &MainWindow::showPropertiesPreview);
}

/**
 * @brief Create Actions for the MainWindow
 *
 */
void MainWindow::createActions(void)
{
    m_quitAction = new QAction( tr( "Quit Ctrl+Q" ), this );
    m_quitAction->setIcon( QIcon( "://images/application-exit.png" ) );
    m_quitAction->setShortcut( QKeySequence::Quit );
    m_quitAction->setStatusTip( tr( "Exit application" ) );

    m_aboutAction = new QAction( tr( "About" ), this );
    m_aboutAction->setIcon( QIcon( "://images/help-about.png" ) );
    //    aboutAction->setShortcut(tr( "Ctrl+A" ));
    m_aboutAction->setStatusTip( tr( "About Kitchen Scale" ) );

    m_settingAction = new QAction( tr( "Settings I" ), this );
    m_settingAction->setIcon( QIcon( "://images/package_settings.png" ) );
    m_settingAction->setShortcut( Qt::Key_I );
    m_settingAction->setStatusTip( tr( "Configure Kitchen Scale" ) );

    m_openPortAction = new QAction( tr("Open port"), this);
    m_openPortAction->setIcon( QIcon( "://images/connect.png"));
    m_openPortAction->setShortcut(Qt::Key_O);
    m_openPortAction->setToolTip(tr("Connect to seriell device  O"));

    m_aboutQt = new QAction(tr("About Qt"), this);
    m_aboutQt->setIcon(QIcon("://images/qt.png"));
}

/**
 * @brief Create MainWindow Toolbar and add Actions
 *
 */
void MainWindow::createToolbar()
{
    QSize size(32,32);

    m_mainToolBar = new QToolBar();
    m_mainToolBar->setObjectName ("Main Toolbar");
    m_mainToolBar->addAction(m_quitAction);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(m_openPortAction);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(m_settingAction);
    m_mainToolBar->setIconSize (size);
    m_mainToolBar->setWindowTitle("Main Toolbar");
    this->addToolBar (m_mainToolBar);

    QList<QAction*> actionList = m_animationPlaylist->actions();
    if(!actionList.isEmpty()){
        m_animationToolBar = new QToolBar("Animation Toolbar");
        foreach (QAction *action, actionList) {
            m_animationToolBar->addAction(action);
        }
        m_animationToolBar->setIconSize(size);
        this->addToolBar(m_animationToolBar);
    }

    m_helpToolBar = new QToolBar( );
    m_helpToolBar->setObjectName("Help Toolbar");
    m_helpToolBar->setLayoutDirection (Qt::RightToLeft);
    m_helpToolBar->addAction( m_aboutAction );
    m_helpToolBar->addAction( m_aboutQt );
    m_helpToolBar->setIconSize(size);
    m_helpToolBar->setWindowTitle("Help Toolbar");
    this->addToolBar (Qt::TopToolBarArea,m_helpToolBar);
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
