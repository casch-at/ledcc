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

AnimationHandler::AnimationHandler()
{
}

AnimationHandler::~AnimationHandler()
{

}

void AnimationHandler::updateAnimation(const AnimationItem *item)
{

}

void AnimationHandler::updateItemToolTip(const Options &aOptions)
{

}

void AnimationHandler::animationDone()
{

}

void AnimationHandler::playAnimations()
{

}
