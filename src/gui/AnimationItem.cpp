#include "AnimationItem.hpp"

AnimationItem::AnimationItem(QListWidget *view, int type):
    QListWidgetItem(view,type)
{
//    initOptions();
}

AnimationItem::AnimationItem(const QString &text, QListWidget *view, int type):
    QListWidgetItem(text,view,type)
{
//    initOptions();
}

AnimationItem *AnimationItem::clone() const
{
    AnimationItem *item = new AnimationItem(text());
    item->setToolTip(toolTip());
    item->setOptions(getOptions());
    return item;
}

