#include "AnimationHandler.hpp"

#include "Draw.hpp"
#include "Lift.hpp"
#include "Firework.hpp"
#include "AxisNailWall.hpp"
#include "WireBoxCenterShrinkGrow.hpp"
#include "WireBoxCornerShrinkGrow.hpp"
#include "Loadbar.hpp"
#include "RandomFiller.hpp"
#include "RandomSpark.hpp"
#include "RandomSparkFlash.hpp"
#include "RandomZLift.hpp"
#include "Wall.hpp"
#include "Rain.hpp"
#include "StringFly.hpp"
#include "AnimationItem.hpp"
#include "AnimationOptions.hpp"

namespace animations {
    const QString SWireBoxCenterShrinkGrow("Wire Box Center Shrink Grow");
    const QString SWireBoxCornerShrinkGrow("Wire Box Corner Shrink Grow");
    const QString SLift("Lift");
    const QString SWall("Wall");
    const QString SFirework("Firework");
    const QString SRain("Rain");
    const QString SRandomSpark("Random Spark");
    const QString SRandomSparkFlash("Random Spark Flash");
    const QString SRandomFiller("Random Filler");
    const QString SRandomZLift("Random Z-Axis Lift");
    const QString SStringFly("String Fly");
    const QString SLoadbar("Loadbar");
    const QString SAxisNailWall("Axis Nail Wall");
    }

using namespace animations;

/*!
 \brief

*/
AnimationHandler::AnimationHandler(QObject *object):
    QObject(object)
{
    setupAnimationItems();
}

/*!
 \brief Destructor

*/
AnimationHandler::~AnimationHandler()
{
    foreach (Animation *a, m_animationHash)
        delete a;
}


/*!
 \brief

 \param aOptions
*/
void AnimationHandler::updateItemToolTip(const Options &aOptions)
{
    QList<QListWidgetItem*> items/* = m_animationPlaylist->selectedItems()*/;
    if(!items.isEmpty())
    {
        AnimationItem *item = dynamic_cast<AnimationItem*>(items.first());
//        item->setOptions(const_cast<Options&>(aOptions));

        m_animationHash.value(item->text())->createAnimationTooltipAsRichText(item);
//        ui->animationPropertiesPreview->createPropertiePreview(
//                    m_animationHash.value( item->text() )->getAnimationPropertiesAsPlainText( item ) );
        if(m_currentAnimation->getName().compare(item->text()) == 0 /*&& createThread->isRunning()*/)
        {
            updateAnimation(item);
        }
    }
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

    m_currentAnimation = m_animationHash.value(item->text());
//    connect(m_createThread,&QThread::started,m_currentAnimation,&animations::Animation::createAnimation);
//    connect(m_currentAnimation, &animations::Animation::done, m_createThread, &QThread::quit);
    //    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    //    connect(createThread , &QThread::finished, createThread, &QThread::deleteLater);
//    connect(m_currentAnimation,&animations::Animation::done,this,&MainWindow::animationDone);

    updateAnimation(item);
//    m_createThread->start();
}
/*!
 \brief

*/
void AnimationHandler::setupAnimationItems()
{
    m_animationHash.insert(SLift,new Lift);
    m_animationHash.insert(SStringFly,new StringFly);
    m_animationHash.insert(SRandomSparkFlash,new RandomSparkFlash);
    m_animationHash.insert(SRandomSpark,new RandomSpark);
    m_animationHash.insert(SRandomFiller,new RandomFiller);
    m_animationHash.insert(SLoadbar,new Loadbar);
    m_animationHash.insert(SAxisNailWall,new AxisNailWall);
    m_animationHash.insert(SWireBoxCenterShrinkGrow,new WireBoxCenterShrinkGrow);
    m_animationHash.insert(SWireBoxCornerShrinkGrow,new WireBoxCornerShrinkGrow);
    m_animationHash.insert(SRandomZLift,new RandomZLift);
    m_animationHash.insert(SRain,new Rain);
    m_animationHash.insert(SWall,new Wall);
    m_animationHash.insert(SFirework,new Firework);

    m_currentAnimation = m_animationHash.value(SStringFly);


    QHash<QString,Animation*>::const_iterator iter = m_animationHash.constBegin();
    QList<AnimationItem*> itemList;
    while(iter != m_animationHash.constEnd()){
        AnimationItem *item = new AnimationItem(iter.key()/*, m_animationList*/); //TODO:: Animation items must somehow be passed to the AnimationListWidget
        Options *options = new Options;
        options->m_speed = iter.value()->getSpeed();
        if(iter.key().compare(SAxisNailWall) == 0){
            options->m_axis =  dynamic_cast<AxisNailWall*>(iter.value())->getAxis();
            options->m_direction = dynamic_cast<AxisNailWall*>(iter.value())->getDirection();
            item->setAvailableAnimationOptions( AnimationOptions::Speed | AnimationOptions::Axis | AnimationOptions::Direction );
        }else if(iter.key().compare(SFirework) == 0){
            options->m_iteration = dynamic_cast<Firework*>(iter.value())->getIterations();
            options->m_leds = dynamic_cast<Firework*>(iter.value())->getParticles();
            item->setAvailableAnimationOptions( AnimationOptions::Speed | AnimationOptions::Iterations | AnimationOptions::Particls );
        }else if(iter.key().compare(SLift) == 0){
            options->m_iteration = dynamic_cast<Lift*>(iter.value())->getIterations();
            options->m_delay = dynamic_cast<Lift*>(iter.value())->getDelay();
            item->setAvailableAnimationOptions( AnimationOptions::Speed | AnimationOptions::Iterations | AnimationOptions::Delay );
        }else if(iter.key().compare(SLoadbar) == 0){
            options->m_axis = dynamic_cast<Loadbar*>(iter.value())->getAxis();
            item->setAvailableAnimationOptions( AnimationOptions::Speed | AnimationOptions::Axis );
        }else if(iter.key().compare(SRain) == 0){
            options->m_iteration = dynamic_cast<Rain*>(iter.value())->getIterations();
            item->setAvailableAnimationOptions( AnimationOptions::Speed | AnimationOptions::Iterations );
        }else if(iter.key().compare(SRandomFiller) == 0){
            options->m_invert = dynamic_cast<RandomFiller*>(iter.value())->getState() == Draw::ON ? true : false;
            item->setAvailableAnimationOptions( AnimationOptions::Speed | AnimationOptions::LedState );
        }else if(iter.key().compare(SRandomSpark) == 0){
            options->m_iteration = dynamic_cast<RandomSpark*>(iter.value())->getIterations();
            options->m_leds = dynamic_cast<RandomSpark*>(iter.value())->getSparks();
            item->setAvailableAnimationOptions( AnimationOptions::Speed | AnimationOptions::Iterations | AnimationOptions::Leds );
        }else if(iter.key().compare(SRandomSparkFlash) == 0){
            options->m_iteration = dynamic_cast<RandomSparkFlash*>(iter.value())->getIterations();
            options->m_leds = dynamic_cast<RandomSparkFlash*>(iter.value())->getLeds();
            item->setAvailableAnimationOptions( AnimationOptions::Speed | AnimationOptions::Iterations | AnimationOptions::Leds );
        }else if(iter.key().compare(SRandomZLift) == 0){
            options->m_iteration = dynamic_cast<RandomZLift*>(iter.value())->getIterations();
            item->setAvailableAnimationOptions( AnimationOptions::Speed | AnimationOptions::Iterations );
        }else if(iter.key().compare(SStringFly) == 0){
            options->m_text = dynamic_cast<StringFly*>(iter.value())->getSToDisplay();
            item->setAvailableAnimationOptions( AnimationOptions::Speed | AnimationOptions::Text );
        }else if(iter.key().compare(SWall) == 0){
            options->m_axis = dynamic_cast<Wall*>(iter.value())->getAxis();
            options->m_direction = dynamic_cast<Wall*>(iter.value())->getDirection();
            item->setAvailableAnimationOptions( AnimationOptions::Speed | AnimationOptions::Axis | AnimationOptions::Direction );
        }else if(iter.key().compare(SWireBoxCenterShrinkGrow) == 0){
            options->m_iteration = dynamic_cast<WireBoxCenterShrinkGrow*>(iter.value())->getIterations();
            options->m_invert = dynamic_cast<WireBoxCenterShrinkGrow*>(iter.value())->getCenterStart();
            item->setAvailableAnimationOptions( AnimationOptions::Speed | AnimationOptions::Iterations | AnimationOptions::CenterStart );
        }else if(iter.key().compare(SWireBoxCornerShrinkGrow) == 0){
            options->m_iteration = dynamic_cast<WireBoxCornerShrinkGrow*>(iter.value())->getIterations();
            item->setAvailableAnimationOptions( AnimationOptions::Speed | AnimationOptions::Iterations );
        }
        item->setOptions(options);

        iter.value()->createAnimationTooltipAsRichText(item);
//        iter.value()->moveToThread(m_createThread); // move animations to own thread
//        connect(iter.value(),&Animation::sendData,m_sender,&Sender::sendAnimation); // connect animation thread with sender thread
        iter.value()->m_abort = false;
        itemList.append(item);
        iter++;
    }
}

/*!
 \brief

 \param item
*/
void AnimationHandler::updateAnimation(const AnimationItem *item)
{
    QString text = item->text();
    Animation *a = m_animationHash.value(text);
    const Options *options = item->getOptions();

    if(text.compare(SLift) == 0){
        dynamic_cast<Lift*>(a)->setDelay(options->m_delay);
        dynamic_cast<Lift*>(a)->setIterations(options->m_iteration);
        dynamic_cast<Lift*>(a)->setSpeed(options->m_speed);
    }else if(text.compare(SRain) == 0){
        dynamic_cast<Rain*>(a)->setSpeed(options->m_speed);
        dynamic_cast<Rain*>(a)->setIterations(options->m_iteration);
    }else if(text.compare(SStringFly) == 0){
        dynamic_cast<StringFly*>(a)->setSToDisplay(options->m_text);
        dynamic_cast<StringFly*>(a)->setSpeed(options->m_speed);
    }else if(text.compare(SWall) == 0){
        dynamic_cast<Wall*>(a)->setSpeed(options->m_speed);
        dynamic_cast<Wall*>(a)->setAxis(options->m_axis);
        dynamic_cast<Wall*>(a)->setDirection(options->m_direction);
    }else if(text.compare(SFirework) == 0){
        dynamic_cast<Firework*>(a)->setSpeed(options->m_speed);
        dynamic_cast<Firework*>(a)->setParticles(options->m_leds);
        dynamic_cast<Firework*>(a)->setIterations(options->m_iteration);
    }else if(text.compare(SRandomSparkFlash) == 0){
        dynamic_cast<RandomSparkFlash*>(a)->setSpeed(options->m_speed);
        dynamic_cast<RandomSparkFlash*>(a)->setIterations(options->m_iteration);
        dynamic_cast<RandomSparkFlash*>(a)->setLeds(options->m_leds);
    }else if(text.compare(SRandomSpark) == 0){
        dynamic_cast<RandomSpark*>(a)->setSpeed(options->m_speed);
        dynamic_cast<RandomSpark*>(a)->setSparks(options->m_leds);
        dynamic_cast<RandomSpark*>(a)->setIterations(options->m_iteration);
    }else if(text.compare(SRandomFiller) == 0){
        dynamic_cast<RandomFiller*>(a)->setSpeed(options->m_speed);
        dynamic_cast<RandomFiller*>(a)->setState(options->m_state);
    }else if(text.compare(SAxisNailWall) == 0){
        dynamic_cast<AxisNailWall*>(a)->setSpeed(options->m_speed);
        dynamic_cast<AxisNailWall*>(a)->setAxis(options->m_axis);
        dynamic_cast<AxisNailWall*>(a)->setDirection(options->m_direction == Draw::Forward ? Draw::Forward : Draw::Backward);
    }else if(text.compare(SLoadbar) == 0){
        dynamic_cast<Loadbar*>(a)->setSpeed(options->m_speed);
        dynamic_cast<Loadbar*>(a)->setAxis(options->m_axis);
    }else if(text.compare(SWireBoxCenterShrinkGrow) == 0){
        dynamic_cast<WireBoxCenterShrinkGrow*>(a)->setSpeed(options->m_speed);
        dynamic_cast<WireBoxCenterShrinkGrow*>(a)->setCenterStart(options->m_invert == 0 ? false : true);
        dynamic_cast<WireBoxCenterShrinkGrow*>(a)->setIterations(options->m_iteration);
    }else if(text.compare(SWireBoxCornerShrinkGrow) == 0){
        dynamic_cast<WireBoxCornerShrinkGrow*>(a)->setSpeed(options->m_speed);
        dynamic_cast<WireBoxCornerShrinkGrow*>(a)->setIterations(options->m_iteration);
    }else if(text.compare(SRandomZLift) == 0){
        dynamic_cast<RandomZLift*>(a)->setSpeed(options->m_speed);
    }
}
