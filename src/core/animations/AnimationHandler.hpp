#ifndef ANIMATIONHANDLER_HPP
#define ANIMATIONHANDLER_HPP
#include "Options.hpp"

#include <QObject>
#include <QHash>

namespace animations {
    class Lift;
    class AxisNailWall;
    class Firework;
    class Loadbar;
    class Rain;
    class RandomFiller;
    class RandomSpark;
    class RandomSparkFlash;
    class RandomZLift;
    class StringFly;
    class Wall;
    class WireBoxCenterShrinkGrow;
    class WireBoxCornerShrinkGrow;
    class Animation;
    class AnimationItem;

    /*!
     \brief Class \a AnimationHandler interface between all animations

    */
    class AnimationHandler : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(Animation* currentAnimation READ currentAnimation WRITE setCurrentAnimation NOTIFY currentAnimationChanged)
    public:
        AnimationHandler(QObject *object = Q_NULLPTR);
        ~AnimationHandler();
        inline Animation* currentAnimation() const  { return m_currentAnimation; }

    Q_SIGNALS:
        void currentAnimationChanged(Animation* arg);

    public Q_SLOTS:
        void updateAnimation(const AnimationItem *item);
        void updateItemToolTip(const Options &aOptions);
        void animationDone(void);
        void playAnimations(void);

        inline void setCurrentAnimation(Animation* arg) {
            if (m_currentAnimation != arg) {
                m_currentAnimation = arg;
                emit currentAnimationChanged(arg);
            }
        }

    private Q_SLOTS:


    private:
        void setupAnimationItems(void);
        void playNextAnimation(const AnimationItem *item);
        Animation *m_currentAnimation;
        QHash<QString,Animation*> m_animationHash;
        Q_DISABLE_COPY(AnimationHandler)
    };

    }
#endif // ANIMATIONHANDLER_HPP
