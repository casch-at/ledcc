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
#include "animations/Draw.hpp"
#include "animations/Lift.hpp"
#include "animations/Firework.hpp"
#include "animations/AxisNailWall.hpp"
#include "animations/WireBoxCenterShrinkGrow.hpp"
#include "animations/WireBoxCornerShrinkGrow.hpp"
#include "animations/Loadbar.hpp"
#include "animations/RandomFiller.hpp"
#include "animations/RandomSpark.hpp"
#include "animations/RandomSparkFlash.hpp"
#include "animations/RandomZLift.hpp"
#include "animations/Wall.hpp"
#include "animations/Rain.hpp"
#include "animations/StringFly.hpp"
#include "AnimationItem.hpp"
#include "Sender.hpp"
#include "PortMessageBox.hpp"
#include "AnimationListWidget.hpp"
#include "AnimationPlayListWidget.hpp"

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
    sdialog(new SettingsDialog),
    m_focusAnimationList(new  QShortcut(QKeySequence(tr("Alt+1")),this)),
    m_focusAnimationPlaylist(new  QShortcut(QKeySequence(tr("Alt+2")),this)),
    m_scSellectAll(new  QShortcut(QKeySequence(tr("Ctrl+A")),this)),
    createThread(new QThread),
    m_animationList(new AnimationListWidget(this)),
    m_animationPlaylist(new AnimationPlayListWidget(this))
{
    ui->setupUi(this);
    setupSenderThread();
    readSettings ();
    createActions ();
    createToolbar ();
    m_animationPlaylist->m_playAction->setDisabled(true);
    m_animationPlaylist->m_stopAction->setDisabled(true);
    setupAnimationItems();
    connectSignals();
    ui->splitter->setStretchFactor(1,2);
    ui->m_animationListLay->addWidget(m_animationList);
    ui->m_animationPlaylistLay->addWidget(m_animationPlaylist);
    m_animationList->setFocus();
    AQP::accelerateWidget (this);  //Give each button a accelerater

}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow(void) //Deinit MainWindow
{
    foreach (Animation *a, animation)
        delete a;
    delete createThread;
    delete m_senderThread;
    delete sdialog;
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
        if(createThread->isRunning() || m_senderThread->isRunning())
            stopThreads();
        saveSettings ();
        sdialog->close();
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
    settings.setValue (SETTINGS::GeometrySettings,saveGeometry ());
}

/**
 * @brief MainWindow::readSettings
 */
void MainWindow::readSettings (void){ //Load geometry of application
    QSettings settings("Schwarz Software Inc.","3D-LED Cube");
    restoreGeometry (settings.value (SETTINGS::GeometrySettings).toByteArray ());
}

/**
 * @author  Christian Schwarzgruber
 * @brief   MainWindow::updateUi gets called when the serial port gets opened or closed
 *          and when the animation playlist gets modified.
 *          Disables/Enables the playbutton and changes the Seriel connect button appropriated
 */
void MainWindow::updateUi(void)
{
    if(m_portOpened)
    {
        if(m_openPortAction->text() == "Open port")
        {
            m_openPortAction->setText(tr("Close port"));
            m_openPortAction->setIcon( QIcon( "://images/disconnect.png"));
            m_openPortAction->setToolTip(tr("Disconnect from seriell device  O"));
        }

        if(m_animationPlaylist->count() && !m_animationPlaylist->m_stopAction->isEnabled())
            m_animationPlaylist->m_playAction->setEnabled(true);
        else
            m_animationPlaylist->m_playAction->setDisabled(true);

    }else
    {
        if(m_openPortAction->text() == "Close port")
        {
            m_openPortAction->setText(tr("Open port"));
            m_openPortAction->setIcon( QIcon( "://images/connect.png"));
            m_openPortAction->setToolTip(tr("Connect to seriell device  O"));
        }
        m_animationPlaylist->m_playAction->setDisabled(true);
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

    currentAnimation = animation.value(item->text());
    connect(createThread,&QThread::started,currentAnimation,&Animation::createAnimation);
    connect(currentAnimation, &Animation::done, createThread, &QThread::quit);
    //    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    //    connect(createThread , &QThread::finished, createThread, &QThread::deleteLater);
    connect(currentAnimation,&Animation::done,this,&MainWindow::animationDone);

    updateAnimation(item);
    createThread->start();
}

void MainWindow::playAnimations()
{
    stopPlay = true;

    if(!m_senderThread->isRunning())
        m_senderThread->start();

    m_animationPlaylist->m_playAction->setDisabled(true);
    m_animationPlaylist->m_stopAction->setEnabled(true);
    playNextAnimation(m_animationPlaylist->getNextAnimation());
}


void MainWindow::animationDone()
{
    disconnect(createThread,&QThread::started,currentAnimation,&Animation::createAnimation);
    disconnect(currentAnimation, &Animation::done, createThread, &QThread::quit);
    disconnect(currentAnimation,&Animation::done,this,&MainWindow::animationDone);

    createThread->wait();

    if(stopPlay)
        playNextAnimation(m_animationPlaylist->getNextAnimation());
    else
        updateUi();

}

void MainWindow::updateItemToolTip(const AnimationOptions::Options &aOptions)
{
    QList<QListWidgetItem*> items = m_animationPlaylist->selectedItems();
    if(!items.isEmpty())
    {
        AnimationItem *item = dynamic_cast<AnimationItem*>(items.first());
        item->setOptions(const_cast<AnimationOptions::Options&>(aOptions));

        animation.value(item->text())->createAnimationTooltipAsRichText(item);
        ui->animationPropertiesPreview->createPropertiePreview(
                    animation.value( item->text() )->getAnimationPropertiesAsPlainText( item ) );
        if(currentAnimation->getName().compare(item->text()) == 0 /*&& createThread->isRunning()*/)
        {
            updateAnimation(item);
        }
    }
}

/**
 * @brief
 *
 * @param animationOptions
 */
void MainWindow::updateAnimation(const AnimationItem *item)
{
    QString text = item->text();
    Animation *a = animation.value(text);
    const AnimationOptions::Options *options = item->getOptions();

    if(text.compare(ANIMATIONS::Lift) == 0){
        dynamic_cast<Lift*>(a)->setDelay(options->delay);
        dynamic_cast<Lift*>(a)->setIterations(options->iteration);
        dynamic_cast<Lift*>(a)->setSpeed(options->speed);
    }else if(text.compare(ANIMATIONS::Rain) == 0){
        dynamic_cast<Rain*>(a)->setSpeed(options->speed);
        dynamic_cast<Rain*>(a)->setIterations(options->iteration);
    }else if(text.compare(ANIMATIONS::StringFly) == 0){
        dynamic_cast<StringFly*>(a)->setSToDisplay(options->text);
        dynamic_cast<StringFly*>(a)->setSpeed(options->speed);
    }else if(text.compare(ANIMATIONS::Wall) == 0){
        dynamic_cast<Wall*>(a)->setSpeed(options->speed);
        dynamic_cast<Wall*>(a)->setAxis(options->axis);
        dynamic_cast<Wall*>(a)->setDirection(options->direction);
    }else if(text.compare(ANIMATIONS::Firework) == 0){
        dynamic_cast<Firework*>(a)->setSpeed(options->speed);
        dynamic_cast<Firework*>(a)->setParticles(options->leds);
        dynamic_cast<Firework*>(a)->setIterations(options->iteration);
    }else if(text.compare(ANIMATIONS::RandomSparkFlash) == 0){
        dynamic_cast<RandomSparkFlash*>(a)->setSpeed(options->speed);
        dynamic_cast<RandomSparkFlash*>(a)->setIterations(options->iteration);
        dynamic_cast<RandomSparkFlash*>(a)->setLeds(options->leds);
    }else if(text.compare(ANIMATIONS::RandomSpark) == 0){
        dynamic_cast<RandomSpark*>(a)->setSpeed(options->speed);
        dynamic_cast<RandomSpark*>(a)->setSparks(options->leds);
        dynamic_cast<RandomSpark*>(a)->setIterations(options->iteration);
    }else if(text.compare(ANIMATIONS::RandomFiller) == 0){
        dynamic_cast<RandomFiller*>(a)->setSpeed(options->speed);
        dynamic_cast<RandomFiller*>(a)->setState(options->state);
    }else if(text.compare(ANIMATIONS::AxisNailWall) == 0){
        dynamic_cast<AxisNailWall*>(a)->setSpeed(options->speed);
        dynamic_cast<AxisNailWall*>(a)->setAxis(options->axis);
        dynamic_cast<AxisNailWall*>(a)->setInvert(options->invert == 0 ? false : true);
    }else if(text.compare(ANIMATIONS::Loadbar) == 0){
        dynamic_cast<Loadbar*>(a)->setSpeed(options->speed);
        dynamic_cast<Loadbar*>(a)->setAxis(options->axis);
    }else if(text.compare(ANIMATIONS::WireBoxCenterShrinkGrow) == 0){
        dynamic_cast<WireBoxCenterShrinkGrow*>(a)->setSpeed(options->speed);
        dynamic_cast<WireBoxCenterShrinkGrow*>(a)->setCenterStart(options->invert == 0 ? false : true);
        dynamic_cast<WireBoxCenterShrinkGrow*>(a)->setIterations(options->iteration);
    }else if(text.compare(ANIMATIONS::WireBoxCornerShrinkGrow) == 0){
        dynamic_cast<WireBoxCornerShrinkGrow*>(a)->setSpeed(options->speed);
        dynamic_cast<WireBoxCornerShrinkGrow*>(a)->setIterations(options->iteration);
    }else if(text.compare(ANIMATIONS::RandomZLift) == 0){
        dynamic_cast<RandomZLift*>(a)->setSpeed(options->speed);
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
    currentAnimation->m_abort = true;
    m_sender->m_abort = true;
    m_animationPlaylist->m_stopAction->setDisabled(true);
    stopPlay = false;
    createThread->quit(); // first quit threads befor wait
    m_senderThread->quit();

    m_senderThread->wait();
    createThread->wait();
    currentAnimation->m_abort = false;
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
                animation.value( item->text() )->getAnimationPropertiesAsPlainText(
                    dynamic_cast<AnimationItem*>(item) ) );
}

/**
 * @brief
 *
 */
void MainWindow::setupAnimationItems(void)
{
    animation.insert(ANIMATIONS::Lift,new Lift);
    animation.insert(ANIMATIONS::StringFly,new StringFly);
    animation.insert(ANIMATIONS::RandomSparkFlash,new RandomSparkFlash);
    animation.insert(ANIMATIONS::RandomSpark,new RandomSpark);
    animation.insert(ANIMATIONS::RandomFiller,new RandomFiller);
    animation.insert(ANIMATIONS::Loadbar,new Loadbar);
    animation.insert(ANIMATIONS::AxisNailWall,new AxisNailWall);
    animation.insert(ANIMATIONS::WireBoxCenterShrinkGrow,new WireBoxCenterShrinkGrow);
    animation.insert(ANIMATIONS::WireBoxCornerShrinkGrow,new WireBoxCornerShrinkGrow);
    animation.insert(ANIMATIONS::RandomZLift,new RandomZLift);
    animation.insert(ANIMATIONS::Rain,new Rain);
    animation.insert(ANIMATIONS::Wall,new Wall);
    animation.insert(ANIMATIONS::Firework,new Firework);

    currentAnimation = animation.value(ANIMATIONS::StringFly);

    AnimationOptions::Options options;

    QHash<QString,Animation*>::const_iterator iter = animation.constBegin();
    while(iter != animation.constEnd()){
        AnimationItem *item = new AnimationItem(iter.key(),m_animationList);
        options.speed = iter.value()->getSpeed();

        if(iter.key().compare(ANIMATIONS::AxisNailWall) == 0){
            options.axis =  dynamic_cast<AxisNailWall*>(iter.value())->getAxis();
            options.invert = dynamic_cast<AxisNailWall*>(iter.value())->getInvert();
        }else if(iter.key().compare(ANIMATIONS::Firework) == 0){
            options.iteration = dynamic_cast<Firework*>(iter.value())->getIterations();
            options.leds = dynamic_cast<Firework*>(iter.value())->getParticles();
        }else if(iter.key().compare(ANIMATIONS::Lift) == 0){
            options.iteration = dynamic_cast<Lift*>(iter.value())->getIterations();
            options.delay = dynamic_cast<Lift*>(iter.value())->getDelay();
        }else if(iter.key().compare(ANIMATIONS::Loadbar) == 0){
            options.axis = dynamic_cast<Loadbar*>(iter.value())->getAxis();
        }else if(iter.key().compare(ANIMATIONS::Rain) == 0){
            options.iteration = dynamic_cast<Rain*>(iter.value())->getIterations();
        }else if(iter.key().compare(ANIMATIONS::RandomFiller) == 0){
            options.invert = dynamic_cast<RandomFiller*>(iter.value())->getState() == Draw::ON ? true : false;
        }else if(iter.key().compare(ANIMATIONS::RandomSpark) == 0){
            options.iteration = dynamic_cast<RandomSpark*>(iter.value())->getIterations();
            options.leds = dynamic_cast<RandomSpark*>(iter.value())->getSparks();
        }else if(iter.key().compare(ANIMATIONS::RandomSparkFlash) == 0){
            options.iteration = dynamic_cast<RandomSparkFlash*>(iter.value())->getIterations();
            options.leds = dynamic_cast<RandomSparkFlash*>(iter.value())->getLeds();
        }else if(iter.key().compare(ANIMATIONS::RandomZLift) == 0){
            options.iteration = dynamic_cast<RandomZLift*>(iter.value())->getIterations();
        }else if(iter.key().compare(ANIMATIONS::StringFly) == 0){
            options.text = dynamic_cast<StringFly*>(iter.value())->getSToDisplay();
        }else if(iter.key().compare(ANIMATIONS::Wall) == 0){
            options.axis = dynamic_cast<Wall*>(iter.value())->getAxis();
            options.direction = dynamic_cast<Wall*>(iter.value())->getDirection();
        }else if(iter.key().compare(ANIMATIONS::WireBoxCenterShrinkGrow) == 0){
            options.iteration = dynamic_cast<WireBoxCenterShrinkGrow*>(iter.value())->getIterations();
            options.invert = dynamic_cast<WireBoxCenterShrinkGrow*>(iter.value())->getCenterStart();
        }else if(iter.key().compare(ANIMATIONS::WireBoxCornerShrinkGrow) == 0){
            options.iteration = dynamic_cast<WireBoxCornerShrinkGrow*>(iter.value())->getIterations();
        }
        item->setOptions(options);
        iter.value()->createAnimationTooltipAsRichText(item);

        iter.value()->moveToThread(createThread); // move animations to own thread

        connect(iter.value(),&Animation::sendData,m_sender,&Sender::sendAnimation); // connect animation thread with sender thread

        iter.value()->m_abort = false;
        iter++;
    }
}

/**
 * @brief Open or close serial port
 *
 */
void MainWindow::openCloseSerialPort(void)
{
    if(!m_senderThread->isRunning())
        m_senderThread->start();
    Q_EMIT openSerialInterface(sdialog->settings()); // call send thread
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
    connect( m_settingAction, &QAction::triggered,sdialog,&QWidget::show);
    connect( m_animationPlaylist->m_playAction, &QAction::triggered , this , &MainWindow::playAnimations);
    connect( m_animationPlaylist->m_stopAction, &QAction::triggered , this , &MainWindow::stopThreads);

    // Animation properties ready connection
    connect( ui->animationAdjustGB , &AnimationOptions::optionsReady , this, &MainWindow::updateItemToolTip);

    // ListWidget shortcut sellect all connections
    connect( m_focusAnimationList, &QShortcut::activated, m_animationList, &ListWidget::focus);
    connect( m_focusAnimationPlaylist, &QShortcut::activated, m_animationPlaylist, &ListWidget::focus);
    connect( m_scSellectAll, &QShortcut::activated, m_animationList, &ListWidget::selectAllItems);
    connect( m_scSellectAll, &QShortcut::activated, m_animationPlaylist, &ListWidget::selectAllItems);

    // ListWidget interconnections
    connect( m_animationList , &AnimationListWidget::addToPlaylist , m_animationPlaylist , &AnimationPlayListWidget::newItem);
    connect( m_animationList , &ListWidget::showPropertiePreview , this , &MainWindow::showPropertiesPreview);
    connect( m_animationPlaylist, &AnimationPlayListWidget::updateUi , this, &MainWindow::updateUi);
    connect( m_animationPlaylist, &AnimationPlayListWidget::displayAnimationOptions, ui->animationAdjustGB, &AnimationOptions::displayAnimationOptions);
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
    m_settingAction->setShortcut(tr( "I" ));
    m_settingAction->setStatusTip( tr( "Configure Kitchen Scale" ) );

    m_openPortAction = new QAction( tr("Open port"), this);
    m_openPortAction->setIcon( QIcon( "://images/connect.png"));
    m_openPortAction->setShortcut(tr("O"));
    m_openPortAction->setToolTip(tr("Connect to seriell device  O"));
}

/**
 * @brief Create MainWindow Toolbar and add Actions
 *
 */
void MainWindow::createToolbar()
{
    QSize size(32,32);


    mainToolBar = new QToolBar();

    mainToolBar->setObjectName ("Main Toolbar");
    mainToolBar->addAction(m_quitAction);
    mainToolBar->addSeparator();

    mainToolBar->addAction(m_openPortAction);
    mainToolBar->addSeparator();

    mainToolBar->addAction(m_settingAction);

    mainToolBar->setIconSize (size);
    mainToolBar->setWindowTitle("Main Toolbar");
    this->addToolBar (mainToolBar);

    animationToolBar = new QToolBar("Animation Toolbar");
    animationToolBar->addAction(m_animationPlaylist->m_playAction);
    animationToolBar->addAction(m_animationPlaylist->m_stopAction);
    animationToolBar->addSeparator();
    animationToolBar->addAction(m_animationPlaylist->m_moveUpAction);
    animationToolBar->addAction(m_animationPlaylist->m_moveDownAction);
    animationToolBar->addAction(m_animationPlaylist->m_duplicateAction);
    animationToolBar->addSeparator();
    animationToolBar->addAction(m_animationPlaylist->m_removeAction);
    animationToolBar->addAction(m_animationPlaylist->m_clearAction);
    animationToolBar->setIconSize(size);
    this->addToolBar(animationToolBar);

    helpToolBar = new QToolBar( );
    helpToolBar->setObjectName ("Help Toolbar");
    helpToolBar->setLayoutDirection (Qt::RightToLeft);
    helpToolBar->addAction (m_aboutAction);
    helpToolBar->setIconSize (size);
    helpToolBar->setWindowTitle("Help Toolbar");
    this->addToolBar (Qt::TopToolBarArea,helpToolBar);
    this->helpToolBar->setObjectName ("Hello");
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
