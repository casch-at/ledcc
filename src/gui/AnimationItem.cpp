#include "AnimationItem.hpp"

AnimationItem::AnimationItem(QListWidget *view, int type):
    QListWidgetItem(view,type)
{

}

AnimationItem::AnimationItem(const QString &text, QListWidget *view, int type):
    QListWidgetItem(text,view,type)
{

}
