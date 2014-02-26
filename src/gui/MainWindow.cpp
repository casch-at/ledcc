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
#include "alt_key.hpp"
#include "aqp.hpp"
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

#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>
#include <QMenuBar>
#include <QToolBar>
#include <QByteArray>
#include <QEventLoop>
#include <QSerialPort>
#include <QShortcut>

//#ifdef _DEBUG_
#include <QDebug>
//#endif

using namespace std;

namespace SETTINGS{
    const QString GeometrySettings("geometry");
    }

namespace ANIMATIONS{
    const QString WireBoxCenterShrinkGrow("Wire Box Center Shrink Grow");
    const QString WireBoxCornerShrinkGrow("Wire Box Corner Shrink Grow");
    const QString Lift("Lift");
    const QString Wall("Wall");
    const QString Firework("Firework");
    const QString Rain("Rain");
    const QString RandomSpark("Random Spark");
    const QString RandomSparkFlash("Random Spark Flash");
    const QString RandomFiller("Random Filler");
    const QString RandomZLift("Random Z-Axis Lift");
    const QString StringFly("String Fly");
    const QString Loadbar("Loadbar");
    const QString AxisNailWall("Axis Nail Wall");
    }

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :  //Init MainWindow
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    sdialog(new SettingsDialog),
    shortCutSA(new  QShortcut(QKeySequence(tr("Ctrl+A")),this)),
    createThread(new QThread)
{
    ui->setupUi(this);
    setupSenderThread();
    readSettings ();
    createActions ();
    createToolbar ();
    playAction->setDisabled(true);
    pauseAction->setDisabled(true);
    setupAnimationItems();
    qRegisterMetaType<SettingsDialog::SerialSettings>("SettingsDialog::SerialSettings");

    AQP::accelerateWidget (this);  //Give each button a accelerater
    connectSignals();
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow(void) //Deinit MainWindow
{
    foreach (Animation *a, animation)
        delete a;
    senderThread->deleteLater();
    senderThread->deleteLater();
    delete createThread;
    delete senderThread;
    delete ui;
}

/**
 * @brief MainWindow::closeEvent
 * @param event
 */
void MainWindow::closeEvent( QCloseEvent *event ) {  //Close application
    if ( okToContinue() ) {
        Q_EMIT okClosePort();
        if(createThread->isRunning() || senderThread->isRunning())
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
 * @return
 */
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
/**
 * @brief MainWindow::resizeEvent
 * @param e
 */
void MainWindow::resizeEvent(QResizeEvent *e){ //Resize Window
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
    if(portOpened){
        if(openPortAction->text() == "Open port"){
            openPortAction->setText(tr("Close port"));
            openPortAction->setIcon( QIcon( "://images/disconnect.png"));
            openPortAction->setToolTip(tr("Disconnect from seriell device  O"));
        }
        if(ui->animationPlaylistLW->count() && !pauseAction->isEnabled()){
            playAction->setEnabled(true);
        }else{
            playAction->setDisabled(true);
        }
    }else
    {
        if(openPortAction->text() == "Close port"){
            openPortAction->setText(tr("Open port"));
            openPortAction->setIcon( QIcon( "://images/connect.png"));
            openPortAction->setToolTip(tr("Connect to seriell device  O"));
        }
        playAction->setDisabled(true);
    }
}

/**
 * @author Christian Schwarzgruber
 * @brief MainWindow::playNextAnimation
 *
 * @param QString &a
 */
void MainWindow::playNextAnimation(const QString &a)
{
    currentAnimation = animation.value(a);
    connect(createThread,&QThread::started,currentAnimation,&Animation::createAnimation);
    connect(currentAnimation, &Animation::done, createThread, &QThread::quit);
    //    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    //    connect(createThread , &QThread::finished, createThread, &QThread::deleteLater);
    connect(currentAnimation,&Animation::done,this,&MainWindow::animationDone);
    createThread->start();
}

void MainWindow::playAnimations()
{
    stopPlay = true;

    if(!senderThread->isRunning())
        senderThread->start();
    playAction->setDisabled(true);
    pauseAction->setEnabled(true);
    getNextAnimation();
}

void MainWindow::getNextAnimation()
{
    QListWidgetItem *item = ui->animationPlaylistLW->getNextAnimation();
    playNextAnimation(item->text());
}

void MainWindow::animationDone()
{
    disconnect(createThread,&QThread::started,currentAnimation,&Animation::createAnimation);
    disconnect(currentAnimation, &Animation::done, createThread, &QThread::quit);
    disconnect(currentAnimation,&Animation::done,this,&MainWindow::animationDone);
    createThread->wait();
    if(stopPlay)
        Q_EMIT getNextAnimation();
    else
        updateUi();

}

void MainWindow::updateItemToolTip(const AnimationOptions::Options *aOptions)
{
    QList<QListWidgetItem*> items = ui->animationPlaylistLW->selectedItems();
    if(!items.isEmpty())
    {
        AnimationItem *item = dynamic_cast<AnimationItem*>(items.first());
        updateAnimationItemToolTip(item,aOptions);
        if(currentAnimation->getName().compare(item->text()) == 0){
            updateAnimation(item->text(),aOptions);
        }

    }
}

/**
 * @brief
 *
 * @param animationOptions
 */
void MainWindow::updateAnimation(const QString &aString,const AnimationOptions::Options *aOptions)
{
    Animation *a = animation.value(aString);
    if(aString.compare(ANIMATIONS::Lift) == 0){
        dynamic_cast<Lift*>(a)->setDelay(aOptions->delay);
        dynamic_cast<Lift*>(a)->setIterations(aOptions->iteration);
        dynamic_cast<Lift*>(a)->setSpeed(aOptions->speed);
    }else if(aString.compare(ANIMATIONS::Rain) == 0){
        dynamic_cast<Rain*>(a)->setSpeed(aOptions->speed);
        dynamic_cast<Rain*>(a)->setIterations(aOptions->iteration);
    }else if(aString.compare(ANIMATIONS::StringFly) == 0){
        dynamic_cast<StringFly*>(a)->setSToDisplay(aOptions->text);
        dynamic_cast<StringFly*>(a)->setSpeed(aOptions->speed);
    }else if(aString.compare(ANIMATIONS::Wall) == 0){
        dynamic_cast<Wall*>(a)->setSpeed(aOptions->speed);
        dynamic_cast<Wall*>(a)->setAxis(aOptions->axis);
        dynamic_cast<Wall*>(a)->setDirection(aOptions->direction);
    }else if(aString.compare(ANIMATIONS::Firework) == 0){
        dynamic_cast<Firework*>(a)->setSpeed(aOptions->speed);
        dynamic_cast<Firework*>(a)->setParticles(aOptions->leds);
        dynamic_cast<Firework*>(a)->setIterations(aOptions->iteration);
    }else if(aString.compare(ANIMATIONS::RandomSparkFlash) == 0){
        dynamic_cast<RandomSparkFlash*>(a)->setSpeed(aOptions->speed);
        dynamic_cast<RandomSparkFlash*>(a)->setIterations(aOptions->iteration);
        dynamic_cast<RandomSparkFlash*>(a)->setLeds(aOptions->leds);
    }else if(aString.compare(ANIMATIONS::RandomSpark) == 0){
        dynamic_cast<RandomSpark*>(a)->setSpeed(aOptions->speed);
        dynamic_cast<RandomSpark*>(a)->setSparks(aOptions->leds);
        dynamic_cast<RandomSpark*>(a)->setIterations(aOptions->iteration);
    }else if(aString.compare(ANIMATIONS::RandomFiller) == 0){
        dynamic_cast<RandomFiller*>(a)->setSpeed(aOptions->speed);
        dynamic_cast<RandomFiller*>(a)->setState(aOptions->state);
    }else if(aString.compare(ANIMATIONS::AxisNailWall) == 0){
        dynamic_cast<AxisNailWall*>(a)->setSpeed(aOptions->speed);
        dynamic_cast<AxisNailWall*>(a)->setAxis(aOptions->axis);
        dynamic_cast<AxisNailWall*>(a)->setInvert(aOptions->invert == 0 ? false : true);
    }else if(aString.compare(ANIMATIONS::Loadbar) == 0){
        dynamic_cast<Loadbar*>(a)->setSpeed(aOptions->speed);
        dynamic_cast<Loadbar*>(a)->setAxis(aOptions->axis);
    }else if(aString.compare(ANIMATIONS::WireBoxCenterShrinkGrow) == 0){
        dynamic_cast<WireBoxCenterShrinkGrow*>(a)->setSpeed(aOptions->speed);
        dynamic_cast<WireBoxCenterShrinkGrow*>(a)->setCenterStart(aOptions->invert == 0 ? false : true);
        dynamic_cast<WireBoxCenterShrinkGrow*>(a)->setIterations(aOptions->iteration);
    }else if(aString.compare(ANIMATIONS::WireBoxCornerShrinkGrow) == 0){
        dynamic_cast<WireBoxCornerShrinkGrow*>(a)->setSpeed(aOptions->speed);
        dynamic_cast<WireBoxCornerShrinkGrow*>(a)->setIterations(aOptions->iteration);
    }else if(aString.compare(ANIMATIONS::RandomZLift) == 0){
        dynamic_cast<RandomZLift*>(a)->setSpeed(aOptions->speed);
    }
}

/**
 * @brief
 *
 * @param a
 * @param item
 */
void MainWindow::updateAnimationItemToolTip(QListWidgetItem *item, const AnimationOptions::Options *a)
{

    QString itemToolTip = QString(tr("<p style='white-space:pre'><font color=#00FFFF><b>%1 Animation</b></font><br>"
                                     "Speed: %2<br>"))
            .arg(item->text())
            .arg(a->speed);
    if(item->text().compare(ANIMATIONS::Lift) == 0){
        itemToolTip.append(QString("Delay: %1<br>"
                                   "Iterations: %2")
                           .arg(a->delay)
                           .arg(a->iteration));
    }else if(item->text().compare(ANIMATIONS::StringFly) == 0){
        itemToolTip.append(QString("Current Text: "
                                   + a->text.toLatin1()));
    }else if(item->text().compare(ANIMATIONS::RandomSparkFlash) == 0){
        itemToolTip.append(QString("LEDs: %1<br>"
                                   "Iterations: %2")
                           .arg(a->leds)
                           .arg(a->iteration));
    }else if(item->text().compare(ANIMATIONS::RandomSpark) == 0){
        itemToolTip.append(QString("Sparks: %1<br>")
                           .arg(a->leds));
    }else if(item->text().compare(ANIMATIONS::RandomFiller) == 0){
        QString tmp;

        tmp  = a->invert == 1 ? "ON" : "OFF";

        itemToolTip.append(QString("Start State: " + tmp));
    }else if(item->text().compare(ANIMATIONS::Loadbar) == 0){
        QString tmp;

        if(a->axis == Draw::X_AXIS)
            tmp = "X-Axis";
        else if(a->axis == Draw::Y_AXIS)
            tmp = "Y-Axis";
        else
            tmp = "Z-Axis";

        itemToolTip.append(QString("Axis: " + tmp));
    }else if(item->text().compare(ANIMATIONS::AxisNailWall) == 0){
        QString tmp;

        if(a->axis == Draw::X_AXIS)
            tmp = "X-Axis<br>";
        else if(a->axis == Draw::Y_AXIS)
            tmp = "Y-Axis<br>";
        else
            tmp = "Z-Axis<br>";

        tmp.append(a->invert == 1 ? "Invert: Yes" : "Invert: No");

        itemToolTip.append(QString("Axis: " + tmp));
    }else if(item->text().compare(ANIMATIONS::WireBoxCenterShrinkGrow) == 0){
        QString tmp;

        tmp  = a->invert == 1 ? "YES<br>" : "NO<br>";
        tmp.append(QString("Iterations: %1")
                   .arg(a->iteration));
        itemToolTip.append(QString("Start in center: " + tmp));
    }else if(item->text().compare(ANIMATIONS::WireBoxCornerShrinkGrow) == 0){
        itemToolTip.append(QString("Iterations: %1")
                           .arg(a->iteration));
    }else if(item->text().compare(ANIMATIONS::RandomZLift) == 0){
        itemToolTip.append(QString("Iterations: %1")
                           .arg(a->iteration));
    }else if(item->text().compare(ANIMATIONS::Rain) == 0){
        itemToolTip.append(QString("Iterations: %1")
                           .arg(a->iteration));
    }else if(item->text().compare(ANIMATIONS::Wall) == 0){
        QString tmp;

        tmp = a->direction == Draw::FORWARD ? "Direction: Forward<br>" : "Direction: Backward<br>";

        if(a->axis == Draw::X_AXIS)
            tmp = "Axis: X-Axis";
        else if(a->axis == Draw::Y_AXIS)
            tmp = "Axis: Y-Axis";
        else
            tmp = "Axis: Z-Axis";

        itemToolTip.append(tmp);
    }else if(item->text().compare(ANIMATIONS::Firework) == 0){
        itemToolTip.append(QString("Iterations: %1<br>"
                                   "Particles: %2")
                           .arg(a->iteration)
                           .arg(a->leds));
    }
    item->setToolTip(itemToolTip);
}

void MainWindow::portOpen(const QString &message)
{
    portOpened = true;
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

void MainWindow::closePort(const QString &message)
{
    PortMessageBox *msg = new PortMessageBox(tr("Close Port"),message,this);

    if(msg->exec() == QMessageBox::Ok)
        Q_EMIT okClosePort();

    delete msg;
}

void MainWindow::portClose(const QString &message)
{
    portOpened = false;
    ui->statusbar->showMessage(message,3000);
    updateUi();
}

void MainWindow::stopThreads()
{
    pauseAction->setDisabled(true);
    currentAnimation->m_abort = true;
    stopPlay = false;
    createThread->quit();
    senderThread->quit();

    senderThread->wait();
    createThread->wait();
    currentAnimation->m_abort = false;
    animationDone();
}

/**
 * @brief
 *
 */
void MainWindow::setupAnimationItems()
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

    QHash<QString,Animation*>::const_iterator iter = animation.constBegin();
    AnimationOptions::Options options;
    while(iter != animation.constEnd()){
        AnimationItem *item = new AnimationItem(iter.key(),ui->availableAnimationsLW);
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
        updateAnimationItemToolTip(item,&options);

        iter.value()->moveToThread(createThread);

        connect(iter.value(),&Animation::sendData,sender,&Sender::sendAnimation);

        iter.value()->m_abort = false;

        iter++;
    }
}

void MainWindow::openCloseSerialPort(void)  // Open the Serial port
{
    Q_EMIT openSerialInterface(sdialog->settings());
}

void MainWindow::setupSenderThread()
{
    portOpened = false;
    senderThread = new QThread;
    sender = new Sender;

    sender->moveToThread(senderThread);
    senderThread->start();

    connect(this,&MainWindow::openSerialInterface,sender,&Sender::openCloseSerialPort);
    connect(sender,&Sender::portOpened,this,&MainWindow::portOpen);
    connect(sender,&Sender::portClosed,this,&MainWindow::portClose);
    connect(sender,&Sender::portError,this,&MainWindow::displayPortErrorMessage);
    connect(sender,&Sender::closePort,this,&MainWindow::closePort);
    connect(this,&MainWindow::okClosePort,sender,&Sender::closeSerialPort);
}

/**
 * @brief
 *
 */
void MainWindow::connectSignals(void) //Connect Signals
{
    connect (openPortAction,&QAction::triggered, this,&MainWindow::openCloseSerialPort);
    connect(quitAction,&QAction::triggered, this,&MainWindow::close);
    connect (aboutAction,&QAction::triggered,this,&MainWindow::about);
    connect(clearAction,&QAction::triggered, ui->animationPlaylistLW,&AnimationPlayListWidget::clearList);
    connect(settingAction,&QAction::triggered,sdialog,&QWidget::show);
    connect(ui->animationPlaylistLW , &AnimationPlayListWidget::updateUi , this, &MainWindow::updateUi);
    connect(ui->availableAnimationsLW , &AnimationListWidget::itemsSelected , ui->animationPlaylistLW , &AnimationPlayListWidget::newItem);
    connect(shortCutSA , &QShortcut::activated,ui->animationPlaylistLW , &AnimationPlayListWidget::selectAllItems);
    connect(shortCutSA , &QShortcut::activated,ui->availableAnimationsLW , &AnimationListWidget::selectAllItems);
    connect(ui->animationAdjustGB , &AnimationOptions::optionsReady , this, &MainWindow::updateItemToolTip);
    connect(playAction , &QAction::triggered , this , &MainWindow::playAnimations);
    connect(pauseAction , &QAction::triggered , this , &MainWindow::stopThreads);

}

/**
 * @brief
 *
 */
void MainWindow::createActions(void)  // Creat action for the toolbar
{
    quitAction = new QAction( tr( "Quit Ctrl+Q" ), this );
    quitAction->setIcon( QIcon( "://images/application-exit.png" ) );
    quitAction->setShortcut( QKeySequence::Quit );
    quitAction->setStatusTip( tr( "Exit application" ) );

    clearAction = new QAction( tr( "Clear F5" ), this );
    clearAction->setIcon( QIcon( "://images/clear.png" ) );
    clearAction->setShortcut( QKeySequence::Refresh );
    clearAction->setStatusTip( tr( "Clear item list" ) );

    aboutAction = new QAction( tr( "About" ), this );
    aboutAction->setIcon( QIcon( "://images/help-about.png" ) );
    //    aboutAction->setShortcut(tr( "Ctrl+A" ));
    aboutAction->setStatusTip( tr( "About Kitchen Scale" ) );

    settingAction = new QAction( tr( "Settings I" ), this );
    settingAction->setIcon( QIcon( "://images/package_settings.png" ) );
    settingAction->setShortcut(tr( "I" ));
    settingAction->setStatusTip( tr( "Configure Kitchen Scale" ) );

    playAction = new QAction( tr("Play Animation R"),this);
    playAction->setIcon(QIcon("://images/media-playback-start-9.png"));
    playAction->setShortcut(tr("R"));
    playAction->setToolTip(tr("Play Animation R"));

    pauseAction = new QAction( tr("Pause Animations P"),this);
    pauseAction->setIcon(QIcon("://images/media-playback-pause-7.png"));
    pauseAction->setShortcut(tr("P"));
    pauseAction->setToolTip(tr("Pause Animations P"));

    openPortAction = new QAction( tr("Open port"), this);
    openPortAction->setIcon( QIcon( "://images/connect.png"));
    openPortAction->setShortcut(tr("O"));
    openPortAction->setToolTip(tr("Connect to seriell device  O"));
}

/**
 * @brief
 *
 */
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

/**
 * @brief
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
