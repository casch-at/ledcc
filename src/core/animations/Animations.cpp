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
#include "Animations.hpp"
#include "Animation.hpp"
#include "AnimationItem.hpp"
#include "AnimationOptions.hpp"
#include "QCoreApplication"


Animations::Animations()
{
    setupAnimationItems();
}

Animations::~Animations()
{
    foreach (Animation *a, m_animationHash)
        delete a;
}


Animation *Animations::get(const QString &key)
{
    return m_animationHash.value(key);
}

Animation *Animations::get(const QString &key, const AnimationItem *defaultValue)
{
    updateAnimation(defaultValue);
    return m_animationHash.value(key);
}

Animation *Animations::get(const AnimationItem *defaultValue)
{
    updateAnimation(defaultValue);
    return m_animationHash.value(defaultValue->text());
}

const QHash<QString, Animation*> * Animations::getAll() const
{
    return &m_animationHash;
}

void Animations::setupAnimationItems()
{
    m_animationHash.insert(BIAS::Lift,new Lift);
    m_animationHash.insert(BIAS::StringFly,new StringFly);
    m_animationHash.insert(BIAS::RandomSparkFlash,new RandomSparkFlash);
    m_animationHash.insert(BIAS::RandomSpark,new RandomSpark);
    m_animationHash.insert(BIAS::RandomFiller,new RandomFiller);
    m_animationHash.insert(BIAS::Loadbar,new Loadbar);
    m_animationHash.insert(BIAS::AxisNailWall,new AxisNailWall);
    m_animationHash.insert(BIAS::WireBoxCenterShrinkGrow,new WireBoxCenterShrinkGrow);
    m_animationHash.insert(BIAS::WireBoxCornerShrinkGrow,new WireBoxCornerShrinkGrow);
    m_animationHash.insert(BIAS::RandomZLift,new RandomZLift);
    m_animationHash.insert(BIAS::Rain,new Rain);
    m_animationHash.insert(BIAS::Wall,new Wall);
    m_animationHash.insert(BIAS::Firework,new Firework);
    m_animationHash.insert(BIAS::Wave,new Wave);

    QHash<QString,Animation*>::const_iterator iter = m_animationHash.constBegin();
    while(iter != m_animationHash.constEnd()){
        AnimationItem *item = new AnimationItem(iter.key());
        AnimationItem::Options options;
        options.m_speed = iter.value()->speed();
        if(iter.key().compare(BIAS::AxisNailWall) == 0){
            options.m_axis =  dynamic_cast<AxisNailWall*>(iter.value())->getAxis();
            options.m_direction = dynamic_cast<AxisNailWall*>(iter.value())->getDirection();
            item->setAvailableAnimationOptions( AnimationItem::Speed | AnimationItem::Axis | AnimationItem::Direction );
        }else if(iter.key().compare(BIAS::Firework) == 0){
            options.m_iteration = dynamic_cast<Firework*>(iter.value())->iterations();
            options.m_leds = dynamic_cast<Firework*>(iter.value())->particles();
            item->setAvailableAnimationOptions( AnimationItem::Speed | AnimationItem::Iterations | AnimationItem::Particls );
        }else if(iter.key().compare(BIAS::Lift) == 0){
            options.m_iteration = dynamic_cast<Lift*>(iter.value())->getIterations();
            options.m_delay = dynamic_cast<Lift*>(iter.value())->getDelay();
            item->setAvailableAnimationOptions( AnimationItem::Speed | AnimationItem::Iterations | AnimationItem::Delay );
        }else if(iter.key().compare(BIAS::Loadbar) == 0){
            options.m_axis = dynamic_cast<Loadbar*>(iter.value())->getAxis();
            item->setAvailableAnimationOptions( AnimationItem::Speed | AnimationItem::Axis );
        }else if(iter.key().compare(BIAS::Rain) == 0){
            options.m_iteration = dynamic_cast<Rain*>(iter.value())->getIterations();
            item->setAvailableAnimationOptions( AnimationItem::Speed | AnimationItem::Iterations );
        }else if(iter.key().compare(BIAS::RandomFiller) == 0){
            options.m_invert = dynamic_cast<RandomFiller*>(iter.value())->getState() == Draw::ON ? true : false;
            item->setAvailableAnimationOptions( AnimationItem::Speed | AnimationItem::LedState );
        }else if(iter.key().compare(BIAS::RandomSpark) == 0){
            options.m_iteration = dynamic_cast<RandomSpark*>(iter.value())->getIterations();
            options.m_leds = dynamic_cast<RandomSpark*>(iter.value())->getSparks();
            item->setAvailableAnimationOptions( AnimationItem::Speed | AnimationItem::Iterations | AnimationItem::Leds );
        }else if(iter.key().compare(BIAS::RandomSparkFlash) == 0){
            options.m_iteration = dynamic_cast<RandomSparkFlash*>(iter.value())->getIterations();
            options.m_leds = dynamic_cast<RandomSparkFlash*>(iter.value())->getLeds();
            item->setAvailableAnimationOptions( AnimationItem::Speed | AnimationItem::Iterations | AnimationItem::Leds );
        }else if(iter.key().compare(BIAS::RandomZLift) == 0){
            options.m_iteration = dynamic_cast<RandomZLift*>(iter.value())->getIterations();
            item->setAvailableAnimationOptions( AnimationItem::Speed | AnimationItem::Iterations );
        }else if(iter.key().compare(BIAS::StringFly) == 0){
            options.m_text = dynamic_cast<StringFly*>(iter.value())->getSToDisplay();
            item->setAvailableAnimationOptions( AnimationItem::Speed | AnimationItem::Text );
        }else if(iter.key().compare(BIAS::Wall) == 0){
            options.m_axis = dynamic_cast<Wall*>(iter.value())->getAxis();
            options.m_direction = dynamic_cast<Wall*>(iter.value())->getDirection();
            item->setAvailableAnimationOptions( AnimationItem::Speed | AnimationItem::Axis | AnimationItem::Direction );
        }else if(iter.key().compare(BIAS::WireBoxCenterShrinkGrow) == 0){
            options.m_iteration = dynamic_cast<WireBoxCenterShrinkGrow*>(iter.value())->getIterations();
            options.m_invert = dynamic_cast<WireBoxCenterShrinkGrow*>(iter.value())->getCenterStart();
            item->setAvailableAnimationOptions( AnimationItem::Speed | AnimationItem::Iterations | AnimationItem::CenterStart );
        }else if(iter.key().compare(BIAS::WireBoxCornerShrinkGrow) == 0){
            options.m_iteration = dynamic_cast<WireBoxCornerShrinkGrow*>(iter.value())->iterations();
            item->setAvailableAnimationOptions( AnimationItem::Speed | AnimationItem::Iterations );
        }
        else if(iter.key().compare(BIAS::Wave) == 0){
            options.m_iteration = dynamic_cast<Wave*>(iter.value())->iterations();
            item->setAvailableAnimationOptions( AnimationItem::Speed | AnimationItem::Iterations );
        }
        item->setOptions(options);
        iter++.value()->m_abort = false;
        item->createAnimationTooltipAsRichText();
        m_animationItemDefaultList.append(item);
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
    const AnimationItem::Options *options = item->getOptions();

    if(text.compare(BIAS::Lift) == 0){
        dynamic_cast<Lift*>(a)->setDelay(options->m_delay);
        dynamic_cast<Lift*>(a)->setIterations(options->m_iteration);
        dynamic_cast<Lift*>(a)->setSpeed(options->m_speed);
    }else if(text.compare(BIAS::Rain) == 0){
        dynamic_cast<Rain*>(a)->setSpeed(options->m_speed);
        dynamic_cast<Rain*>(a)->setIterations(options->m_iteration);
    }else if(text.compare(BIAS::StringFly) == 0){
        dynamic_cast<StringFly*>(a)->setSToDisplay(options->m_text);
        dynamic_cast<StringFly*>(a)->setSpeed(options->m_speed);
    }else if(text.compare(BIAS::Wall) == 0){
        dynamic_cast<Wall*>(a)->setSpeed(options->m_speed);
        dynamic_cast<Wall*>(a)->setAxis(options->m_axis);
        dynamic_cast<Wall*>(a)->setDirection(options->m_direction);
    }else if(text.compare(BIAS::Firework) == 0){
        dynamic_cast<Firework*>(a)->setSpeed(options->m_speed);
        dynamic_cast<Firework*>(a)->setParticles(options->m_leds);
        dynamic_cast<Firework*>(a)->setIterations(options->m_iteration);
    }else if(text.compare(BIAS::RandomSparkFlash) == 0){
        dynamic_cast<RandomSparkFlash*>(a)->setSpeed(options->m_speed);
        dynamic_cast<RandomSparkFlash*>(a)->setIterations(options->m_iteration);
        dynamic_cast<RandomSparkFlash*>(a)->setLeds(options->m_leds);
    }else if(text.compare(BIAS::RandomSpark) == 0){
        dynamic_cast<RandomSpark*>(a)->setSpeed(options->m_speed);
        dynamic_cast<RandomSpark*>(a)->setSparks(options->m_leds);
        dynamic_cast<RandomSpark*>(a)->setIterations(options->m_iteration);
    }else if(text.compare(BIAS::RandomFiller) == 0){
        dynamic_cast<RandomFiller*>(a)->setSpeed(options->m_speed);
        dynamic_cast<RandomFiller*>(a)->setState(options->m_state);
    }else if(text.compare(BIAS::AxisNailWall) == 0){
        dynamic_cast<AxisNailWall*>(a)->setSpeed(options->m_speed);
        dynamic_cast<AxisNailWall*>(a)->setAxis(options->m_axis);
        dynamic_cast<AxisNailWall*>(a)->setDirection(options->m_direction);
    }else if(text.compare(BIAS::Loadbar) == 0){
        dynamic_cast<Loadbar*>(a)->setSpeed(options->m_speed);
        dynamic_cast<Loadbar*>(a)->setAxis(options->m_axis);
    }else if(text.compare(BIAS::WireBoxCenterShrinkGrow) == 0){
        dynamic_cast<WireBoxCenterShrinkGrow*>(a)->setSpeed(options->m_speed);
        dynamic_cast<WireBoxCenterShrinkGrow*>(a)->setCenterStart(options->m_invert);
        dynamic_cast<WireBoxCenterShrinkGrow*>(a)->setIterations(options->m_iteration);
    }else if(text.compare(BIAS::WireBoxCornerShrinkGrow) == 0){
        dynamic_cast<WireBoxCornerShrinkGrow*>(a)->setSpeed(options->m_speed);
        dynamic_cast<WireBoxCornerShrinkGrow*>(a)->setIterations(options->m_iteration);
    }else if(text.compare(BIAS::RandomZLift) == 0){
        dynamic_cast<RandomZLift*>(a)->setSpeed(options->m_speed);
    }else if(text.compare(BIAS::Wave) == 0){
        dynamic_cast<Wave*>(a)->setSpeed(options->m_speed);
        dynamic_cast<Wave*>(a)->setIterations(options->m_iteration);
    }
}
