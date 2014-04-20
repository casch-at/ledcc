#include "Animations.hpp"
#include "Animation.hpp"
#include "AnimationItem.hpp"
#include "AnimationOptions.hpp"
#include "QCoreApplication"

using namespace BIAS;

Animations* Animations::m_instance(Q_NULLPTR);

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

Animations *Animations::instance()
{
    if (!m_instance) {
        m_instance = new Animations(qApp);
    }
    return m_instance;
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

/*!
 \brief

 \param item
*/
void Animations::updateAnimation(const AnimationItem *item)
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
        dynamic_cast<AxisNailWall*>(a)->setDirection(options->m_direction);
    }else if(text.compare(SLoadbar) == 0){
        dynamic_cast<Loadbar*>(a)->setSpeed(options->m_speed);
        dynamic_cast<Loadbar*>(a)->setAxis(options->m_axis);
    }else if(text.compare(SWireBoxCenterShrinkGrow) == 0){
        dynamic_cast<WireBoxCenterShrinkGrow*>(a)->setSpeed(options->m_speed);
        dynamic_cast<WireBoxCenterShrinkGrow*>(a)->setCenterStart(options->m_invert);
        dynamic_cast<WireBoxCenterShrinkGrow*>(a)->setIterations(options->m_iteration);
    }else if(text.compare(SWireBoxCornerShrinkGrow) == 0){
        dynamic_cast<WireBoxCornerShrinkGrow*>(a)->setSpeed(options->m_speed);
        dynamic_cast<WireBoxCornerShrinkGrow*>(a)->setIterations(options->m_iteration);
    }else if(text.compare(SRandomZLift) == 0){
        dynamic_cast<RandomZLift*>(a)->setSpeed(options->m_speed);
    }
}
