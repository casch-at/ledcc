#ifndef ANIMATIONLISTWIDGET_HPP
#define ANIMATIONLISTWIDGET_HPP

#include <QListWidget>
#include "Global.hpp"
class AnimationListWidget : public QListWidget
{
    Q_OBJECT
public:
    AnimationListWidget(QWidget* parent = Q_NULLPTR);
Q_SIGNALS:
    void itemsSelected(QList<QListWidgetItem *> selected);
protected:
    virtual void 	keyPressEvent(QKeyEvent * event);
public Q_SLOTS:
    void selectAllItems(void);
//    QHash<QString,AnimationStruct>* getAnimationList(void);
//    void setAnimationList(QHash<QString,AnimationStruct> &animations);
private:
    QList<QListWidgetItem *> selected;
//    QHash<QString,AnimationStruct> alist;
};

#endif // ANIMATIONLISTWIDGET_HPP
