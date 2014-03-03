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

    virtual AnimationItem *clone() const;
//    AnimationItem(const AnimationItem &other);
//    ~AnimationItem();
    AnimationOptions::Options getOptions(void) const{
        return m_options;
    }
    void setOptions(const AnimationOptions::Options &options){
        m_options = options;
    }

private:
//    void initOptions(void);

    AnimationOptions::Options m_options;

};

#endif // ANIMATIONITEM_HPP
