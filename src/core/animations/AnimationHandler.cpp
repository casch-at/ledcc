#include "AnimationHandler.hpp"

#include "Animation.hpp"
#include "Animations.hpp"
#include "AnimationItem.hpp"
#include "AnimationPlayListWidget.hpp"

using namespace animations;

/*!
 \brief

*/
AnimationHandler::AnimationHandler(const Animations *animations, QObject *object):
    QObject(object),
    m_buildInAnimations(animations)
{

}

/*!
 \brief Destructor

*/
AnimationHandler::~AnimationHandler()
{
    delete m_buildInAnimations;
}




/*!
 \brief

*/
void AnimationHandler::animationDone()
{
//    disconnect(m_createThread,&QThread::started,m_currentAnimation,&Animation::createAnimation);
//    disconnect(m_currentAnimation, &Animation::done, m_createThread, &QThread::quit);
//    disconnect(m_currentAnimation,&Animation::done,this,&MainWindow::animationDone);

//    m_createThread->wait();

//    if(m_stopPlay)
//        playNextAnimation(m_animationPlaylist->getNextAnimation());
//    else
//        updateUi();
}

/*!
 \brief

*/
void AnimationHandler::playAnimations()
{
//    m_stopPlay = true;

//    if(!m_senderThread->isRunning())
//        m_senderThread->start();

//    m_animationPlaylist->m_playAction->setDisabled(true);
//    m_animationPlaylist->m_stopAction->setEnabled(true);
//    playNextAnimation(m_animationPlaylist->getNextAnimation());
}

/**
 * @author Christian Schwarzgruber
 * @brief MainWindow::playNextAnimation
 *
 * @param QString &a
 */
void AnimationHandler::playNextAnimation(const AnimationItem *item)
{
    if(item == Q_NULLPTR){
        Q_EMIT stopPlay();
        return;
    }
//    m_playlist->m_currentAnimation
//    m_currentAnimation = m_animationHash.value(item->text());
//    connect(m_createThread,&QThread::started,m_currentAnimation,&animations::Animation::createAnimation);
//    connect(m_currentAnimation, &animations::Animation::done, m_createThread, &QThread::quit);
    //    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    //    connect(createThread , &QThread::finished, createThread, &QThread::deleteLater);
//    connect(m_currentAnimation,&animations::Animation::done,this,&MainWindow::animationDone);

//    updateAnimation(item);
//    m_createThread->start();
}
/*!
 \brief

*/
