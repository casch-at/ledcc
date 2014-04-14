#ifndef ANIMATIONHANDLER_HPP
#define ANIMATIONHANDLER_HPP
#include <QObject>

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

    class AnimationHandler : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(Animation* currentAnimation READ currentAnimation WRITE setCurrentAnimation NOTIFY currentAnimationChanged)
    public:
        AnimationHandler();
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
        void playNextAnimation(const AnimationItem *item);
        Animation *m_currentAnimation;
    };

    }
#endif // ANIMATIONHANDLER_HPP
