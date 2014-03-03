#ifndef ANIMATIONITEM_HPP
#define ANIMATIONITEM_HPP

#include <QListWidgetItem>
#include "AnimationOptions.hpp"

class AnimationItem : public QListWidgetItem
{
//    Q_OBJECT
public:
    explicit AnimationItem(QListWidget *view = 0, int type = Type);
    explicit AnimationItem(const QString &text, QListWidget *view = 0, int type = Type);

//    AnimationItem(const AnimationItem &other);
//    ~AnimationItem();
    AnimationOptions::Options *getOptions(void){
        return &m_options;
    }
    void setOptions(AnimationOptions::Options &options){
        m_options = options;
    }

private:
    AnimationOptions::Options m_options;
};

#endif // ANIMATIONITEM_HPP
