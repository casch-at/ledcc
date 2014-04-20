#ifndef ANIMATIONS_HPP
#define ANIMATIONS_HPP

#include <QObject>
#include <QHash>

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

/*! Build in animations */
namespace BIAS {
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


class Animation;
class AnimationItem;

class Animations : public QObject
{
    Q_OBJECT
public:
    ~Animations();
    Animation* get(const QString& key);
    Animation* get(const QString& key,const AnimationItem* defaultValue);
    static Animations* instance();
    QList<AnimationItem*> const * animationItemDefaultList() const { return &m_animationItemDefaultList; }
//    QHash<QString, Animation*> const *animationHash() const { return &m_animationHash; }
    void updateAnimation(const AnimationItem *item);
signals:

public slots:

private:
    explicit Animations(QObject *parent);

    static Animations *m_instance;

    void setupAnimationItems(void);
    QHash<QString, Animation*> m_animationHash;
    QList<AnimationItem*> m_animationItemDefaultList;
    Q_DISABLE_COPY(Animations)
};

inline Animations* animations(){
    return Animations::instance();
}

#endif // ANIMATIONS_HPP
