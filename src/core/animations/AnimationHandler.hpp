#ifndef ANIMATIONHANDLER_HPP
#define ANIMATIONHANDLER_HPP
#include "Options.hpp"

#include <QObject>
#include <QHash>
//#include "AnimationListWidget.hpp"
//#include "AnimationListWidget.hpp"
class AnimationPlayListWidget;
namespace animations {
    class Animation;
    class AnimationItem;
    class Animations;

    /*!
     \brief Class \a AnimationHandler interface between all animations

    */

    class AnimationHandler : public QObject
    {
        Q_OBJECT
    public:
        AnimationHandler(const Animations *animations, QObject *object = Q_NULLPTR);
        ~AnimationHandler();

    Q_SIGNALS:
        void startAnimation();
        void stopPlay();
        void updateUi();
    public Q_SLOTS:
        void animationDone(void);
        void playAnimations(void);
    private Q_SLOTS:


    private:
        AnimationPlayListWidget *m_playlist;
        const Animations *m_buildInAnimations;
        void playNextAnimation(const AnimationItem *item);
        friend class AnimationPlayListWidget;
        Q_DISABLE_COPY(AnimationHandler)
    };

    }
#endif // ANIMATIONHANDLER_HPP
