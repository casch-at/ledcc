#ifndef ANIMATIONPLAYLISTWIDGET_HPP
#define ANIMATIONPLAYLISTWIDGET_HPP

#include <QListWidget>
#include "AnimationItem.hpp"

class AnimationPlayListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit AnimationPlayListWidget(QWidget *parent = Q_NULLPTR);

Q_SIGNALS:
    void updateUi(void);
    void displayAnimationOptions(const AnimationOptions::Options options);
public Q_SLOTS:
    void clearList(void);
    void newItem(QList<QListWidgetItem *> item);
    void on_itemDoubleClicked(QListWidgetItem *item);
protected:
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void dragMoveEvent(QDragMoveEvent *e);
    virtual void dragLeaveEvent(QDragLeaveEvent *e);
    virtual void dropEvent(QDropEvent *event);
public Q_SLOTS:
    void selectAllItems(void);
    AnimationItem *getNextAnimation(void);
private:
};

#endif // ANIMATIONPLAYLISTWIDGET_HPP
