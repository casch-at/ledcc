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
#include "AnimationHandler.hpp"

#include "Animation.hpp"
#include "Animations.hpp"
#include "AnimationItem.hpp"
#include "AnimationPlayListWidget.hpp"
#include "Sender.hpp"
#include "PortMessageBox.hpp"
#include "SettingsDialog.hpp"
#include <QApplication>
#include <QString>
#include <QAction>

using namespace BIAS;

/*!
 \brief

*/
AnimationHandler::AnimationHandler(QObject *object, QWidget *parent):
    QObject(object),
    m_settingsDialog(new SettingsDialog(this)),
    m_createThread(new QThread)
{
    m_currentAnimation = animations()->get("String Fly");
    setupSenderThread();
    connect(m_sender, &Sender::portOpenChanged,this,&AnimationHandler::setIsPortOpen);
}

/*!
 \brief Destructor

*/
AnimationHandler::~AnimationHandler()
{
    delete m_settingsDialog;
    Q_EMIT okClosePort();
    if(m_createThread->isRunning() || m_senderThread->isRunning())
        stopThreads();
}

void AnimationHandler::setAction(QAction *action, int i)
{
    if (i) {
        m_playAction = action;
    } else {
        m_stopAction = action;
    }
}


/*!
 \brief

*/
void AnimationHandler::animationDone()
{
    disconnect(m_createThread,&QThread::started,m_currentAnimation,&Animation::createAnimation);
    disconnect(m_currentAnimation, &Animation::done, m_createThread, &QThread::quit);
    disconnect(m_currentAnimation,&Animation::done,this,&AnimationHandler::animationDone);

    m_createThread->wait();

    if(m_stopPlay)
        playNextAnimation(m_animationPlaylist->getNextAnimation());
    else
        updateUi();
}

/*!
 \brief

*/
void AnimationHandler::playAnimations()
{
    m_stopPlay = true;

    if(!m_senderThread->isRunning())
        m_senderThread->start();

    m_playAction->setDisabled(true);//FIXME::
    m_stopAction->setEnabled(true);//FIXME::
    playNextAnimation(m_animationPlaylist->getNextAnimation());
}

/**
 * @author Christian Schwarzgruber
 * @brief AnimationHandler::playNextAnimation
 *
 * @param QString &a
 */
void AnimationHandler::playNextAnimation(const AnimationItem *item)
{
    if(item == Q_NULLPTR){
        Q_EMIT stopPlay();
        return;
    }
    m_currentAnimation = animations()->get(item->text());
    connect(m_createThread,&QThread::started,m_currentAnimation,&Animation::createAnimation);
    connect(m_currentAnimation, &Animation::done, m_createThread, &QThread::quit);
    //        connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    //        connect(createThread , &QThread::finished, createThread, &QThread::deleteLater);
    connect(m_currentAnimation,&Animation::done,this,&AnimationHandler::animationDone);

    animations()->updateAnimation(item);
    m_createThread->start();
}
/*!
 \brief

*/


void AnimationHandler::portOpen(const QString &message)
{

    //    m_statusbar->showMessage(message,3000);//FIXME::
    updateUi();
}

void AnimationHandler::displayPortErrorMessage(const QString &message)
{
    PortMessageBox *msg = Q_NULLPTR;
    if(message.contains("#")){
        msg = new PortMessageBox(tr("Error"),message.split("#").first(),message.split("#").last()); //FIXME::
    }else{
        msg = new PortMessageBox(tr("Error"),message);//FIXME::
    }
    msg->exec();
    delete msg;
}

/**
 * @brief Gets called when user presses the port close Button and the port is open
 *
 * @param message to show
 */
void AnimationHandler::closePort(const QString &message)
{
    PortMessageBox *msg = new PortMessageBox(tr("Close Port"),message); //FIXME::

    if(msg->exec() == QMessageBox::Ok)
        Q_EMIT okClosePort();

    delete msg;
}

/**
 * @brief Gets called when the serial port gets closed
 *
 * @param message to show
 */
void AnimationHandler::portClosed(const QString &message)
{
    //    ui->m_statusbar->showMessage(message,3000); //FIXME::
    stopThreads();
    updateUi();
}

/**
 * @brief Stop sender and animation Thread
 *
 */
void AnimationHandler::stopThreads()
{
    bool senderRunning = m_senderThread->isRunning();
    bool createrRunning = m_createThread->isRunning();
    if(!senderRunning && !createrRunning)
        return;
    m_stopAction->setDisabled(true); //FIXME::
    if (m_currentAnimation)
        m_currentAnimation->m_abort = true;
    m_sender->m_abort = true;
    m_stopPlay = false;
    if (createrRunning)
        m_createThread->quit(); // first quit threads befor wait
    if (senderRunning)
        m_senderThread->quit();
    if(createrRunning)
        m_senderThread->wait();
    if (senderRunning)
        m_createThread->wait();
    if (m_currentAnimation)
        m_currentAnimation->m_abort = false;

    m_sender->m_abort = false;
    animationDone();
}

/**
 * @brief Setup sender Thread and create related connections
 *
 */
void AnimationHandler::setupSenderThread(void)
{
    m_senderThread = new QThread;
    m_sender = new Sender;

    m_sender->moveToThread(m_senderThread); // move data send class to own thread
    m_senderThread->start(); // start sender thread, which is running for the entier live of the AnimationHandler

    connect(this,&AnimationHandler::openSerialInterface,m_sender,&Sender::openCloseSerialPort);
    connect(m_sender,&Sender::portOpened,this,&AnimationHandler::portOpen);
    connect(m_sender,&Sender::portClosed,this,&AnimationHandler::portClosed);
    connect(m_sender,&Sender::portError,this,&AnimationHandler::displayPortErrorMessage);
    connect(m_sender,&Sender::closePort,this,&AnimationHandler::closePort);
    connect(this,&AnimationHandler::okClosePort,m_sender,&Sender::closeSerialPort);
    QHashIterator<QString, Animation*> i(*animations()->getAll());
    while (i.hasNext()) {
        i.next();
        connect(i.value(),&Animation::sendData, m_sender, &Sender::sendAnimation);
    }

}

/**
 * @brief Open or close serial port
 *
 */
void AnimationHandler::openCloseSerialPort(void)
{
    if(!m_senderThread->isRunning())
        m_senderThread->start();
    Q_EMIT openSerialInterface(); // call send thread
}
