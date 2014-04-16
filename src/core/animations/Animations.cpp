#include "Animations.hpp"
#include "Animation.hpp"
#include "AnimationItem.hpp"
#include "AnimationOptions.hpp"

// All available build in animations
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

using namespace animations;

Animations::Animations(QObject *parent) :
    QObject(parent)
{
    setupAnimationItems();
}

Animations::~Animations()
{
    foreach (Animation *a, m_animationHash)
        delete a;
}


void Animations::setupAnimationItems()
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

    QHash<QString,Animation*>::const_iterator iter = m_animationHash.constBegin();
    while(iter != m_animationHash.constEnd()){
        AnimationItem *item = new AnimationItem(iter.key());
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
        iter.value()->createAnimationTooltipAsRichText(item); //FIXME:: Connection between creater and sender thread must be created
        iter.value()->m_abort = false;
        m_animationItemDefaultList.append(item);
        iter++;
    }
}
