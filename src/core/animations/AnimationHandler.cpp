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

/* Application includes */
#include "Animation.hpp"
#include "Animations.hpp"
#include "AnimationItem.hpp"
#include "AnimationPlaylistWidget.hpp"
#include "Sender.hpp"
#include "PortMessageBox.hpp"
#include "SettingsDialog.hpp"

/* Qt includes */
#include <QApplication>
#include <QString>
#include <QAction>
#include <QStatusBar>

#ifdef _DEBUG_
#include <QDebug>
#endif
/*!
 \brief

*/
AnimationHandler::AnimationHandler(QWidget *widget):
    QObject(widget->parent()),
    m_settingsDialog(new SettingsDialog(widget)),
    m_createThread(new QThread),
    m_animations(new Animations),
    m_isPortOpen(false)
{
    m_playAction = Q_NULLPTR;
    m_stopAction = Q_NULLPTR;
    m_animationPlaylist = Q_NULLPTR;

    m_playAction = widget->findChild<QAction*>("m_playAction");
    Q_ASSERT(m_playAction);

    m_stopAction = widget->findChild<QAction*>("m_stopAction");
    Q_ASSERT(m_stopAction);

    m_animationPlaylist = widget->findChild<AnimationPlaylistWidget *>("m_animationPlaylist");
    Q_ASSERT(m_animationPlaylist);
    m_currentAnimation = m_animations->get(BIAS::StringFly);
    setupSenderThread();
    connect(m_sender, &Sender::portOpenChanged,this,&AnimationHandler::setIsPortOpen);
    connect(m_animationPlaylist, &AnimationPlaylistWidget::playAnimation, this, &AnimationHandler::playAnimation);
}

/*!
 \brief Destructor

*/
AnimationHandler::~AnimationHandler()
{
    delete m_settingsDialog;
    Q_EMIT okClosePort();
    if(m_createThread->isRunning() || m_senderThread->isRunning()){
        m_currentAnimation->m_abort = true;
        m_sender->m_abort = true;

        /* Tell the thread they shoulde exit with code 0. */
        m_senderThread->quit();
        m_createThread->quit();

        /* Wait untile the thread exit */
        m_senderThread->wait();
        m_createThread->wait();
    }
    delete m_animations;
    delete m_createThread;
    delete m_sender;
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

    playNextAnimation(m_animationPlaylist->getNextAnimation());
}

/*!
 \brief

*/
void AnimationHandler::playAnimations()
{
    startSenderThread();
    playNextAnimation(m_animationPlaylist->getNextAnimation());
}

void AnimationHandler::playAnimation(const AnimationItem *animation)
{
    if (m_isPortOpen) {
        if (m_createThread->isRunning())
            stopThreads();
        startSenderThread();
        playNextAnimation(animation);
    }
}


/**
 * @author Christian Schwarzgruber
 * @brief AnimationHandler::playNextAnimation
 *
 * @param QString &a
 */
void AnimationHandler::playNextAnimation(const AnimationItem *item)
{
    if(item){
        m_currentAnimation = m_animations->get(item->text());
        connect(m_createThread,&QThread::started,m_currentAnimation,&Animation::createAnimation);
        connect(m_currentAnimation, &Animation::done, m_createThread, &QThread::quit);
        connect(m_currentAnimation,&Animation::done,this,&AnimationHandler::animationDone);

        m_animations->updateAnimation(item);
        m_createThread->start();
    } else  {
        stopThreads();
    }
}
/*!
 \brief

*/


void AnimationHandler::portOpen(const QString &message)
{
    QStatusBar *m_statusbar = this->parent()->findChild<QStatusBar*>("m_statusbar");
    if ( m_statusbar )
        m_statusbar->showMessage(message,3000);
    Q_EMIT updateUi(m_isPortOpen);
}

void AnimationHandler::displayPortErrorMessage(const QString &message)
{
    PortMessageBox *msg = Q_NULLPTR;
    if(message.contains("#")){
        msg = new PortMessageBox(tr("Error"),message.split("#").first(),message.split("#").last());
    }else{
        msg = new PortMessageBox(tr("Error"),message);
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
    PortMessageBox *msg = new PortMessageBox(tr("Close Port"),message);
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
    QStatusBar *m_statusbar = this->parent()->findChild<QStatusBar*>("m_statusbar");
    if ( m_statusbar )
        m_statusbar->showMessage(message,3000);
    stopThreads();
    Q_EMIT updateUi(m_isPortOpen);
}

/**
 * @brief Stop sender and animation Thread
 *
 */
void AnimationHandler::stopThreads()
{

    m_stopAction->setDisabled(true);

    /* Tell the thread that he shoulde stop working */
    m_currentAnimation->m_abort = true;
    m_sender->m_abort = true;

    /* Tell the thread they shoulde exit with code 0. */
    m_senderThread->quit();
    m_createThread->quit();

    /* Wait untile the thread exit */
    m_senderThread->wait();
    m_createThread->wait();

    /* Reset the stop flag */
    m_sender->m_abort = false;
    m_currentAnimation->m_abort = false;

    disconnect(m_createThread,&QThread::started,m_currentAnimation,&Animation::createAnimation);
    disconnect(m_currentAnimation, &Animation::done, m_createThread, &QThread::quit);
    disconnect(m_currentAnimation,&Animation::done,this,&AnimationHandler::animationDone);

    Q_EMIT updateUi(m_isPortOpen);
}



/**
 * @brief Setup sender Thread and create related connections
 *
 */
void AnimationHandler::setupSenderThread(void)
{
    m_senderThread = new QThread;
    m_sender = new Sender;

    m_sender->moveToThread(m_senderThread); // Move Sender class his own thread
    m_senderThread->start();

    connect(this,&AnimationHandler::openSerialInterface,m_sender,&Sender::openCloseSerialPort);
    connect(m_sender,&Sender::portOpened,this,&AnimationHandler::portOpen);
    connect(m_sender,&Sender::portClosed,this,&AnimationHandler::portClosed);
    connect(m_sender,&Sender::portError,this,&AnimationHandler::displayPortErrorMessage);
    connect(m_sender,&Sender::closePort,this,&AnimationHandler::closePort);
    connect(this,&AnimationHandler::okClosePort,m_sender,&Sender::closeSerialPort);
    /*
     * Connect the animation creater thread with the sender thread which
     * will be called when the creater thread emits sendData.
     * Also move all build in animations to the creater thread.
     */
    QHashIterator<QString, Animation*> i(*m_animations->getAll());
    while (i.hasNext()) {
        i.next();
        connect(i.value(),&Animation::sendData, m_sender, &Sender::sendAnimation); /* Create connection between animation (createThread) thread and sender thred */
        i.value()->moveToThread(m_createThread); /* Move animation to createThread */
    }

}

void AnimationHandler::startSenderThread()
{
    if(!m_senderThread->isRunning())
        m_senderThread->start();
    m_playAction->setDisabled(true);
    m_stopAction->setEnabled(true);

}

void AnimationHandler::stopCreaterThread()
{
    if (m_currentAnimation)
        m_currentAnimation->m_abort = true;

    if (m_createThread->isRunning()) {
        m_createThread->quit(); // Always quit the thread befor waiting for them to quit
        m_createThread->wait();
    }

    if (m_currentAnimation)
        m_currentAnimation->m_abort = false;
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
