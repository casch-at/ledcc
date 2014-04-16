#ifndef ANIMATIONS_HPP
#define ANIMATIONS_HPP

#include <QObject>
#include <QHash>
namespace animations {
    class Animation;
    class AnimationItem;

    class Animations : public QObject
    {
        Q_OBJECT
    public:
        explicit Animations(QObject *parent = 0);
        ~Animations();

        QHash<QString, Animation*> const *animationHash() const { return &m_animationHash; }
        QList<AnimationItem*> const * animationItemDefaultList() const { return &m_animationItemDefaultList; }
    signals:

    public slots:

    private:
        void setupAnimationItems(void);
        QHash<QString, Animation*> m_animationHash;
        QList<AnimationItem*> m_animationItemDefaultList;
    };

    }
#endif // ANIMATIONS_HPP
