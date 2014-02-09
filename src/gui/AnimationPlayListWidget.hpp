#ifndef ANIMATIONPLAYLISTWIDGET_HPP
#define ANIMATIONPLAYLISTWIDGET_HPP

#include <QListWidget>
#include "Global.hpp"

class AnimationPlayListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit AnimationPlayListWidget(QWidget *parent = Q_NULLPTR);

Q_SIGNALS:
    void updateUi(void);
public Q_SLOTS:
    void clearList(void);
    void newItem(QList<QListWidgetItem *> item);
protected:
    virtual void keyPressEvent(QKeyEvent * event);
public Q_SLOTS:
    void selectAllItems(void);
//    QHash<QString,AnimationStruct>* getPlaylist(void);
//    const AnimationStruct getNextAnimation(void);
private:
//    QHash<QString,AnimationStruct> playList;
};

#endif // ANIMATIONPLAYLISTWIDGET_HPP
