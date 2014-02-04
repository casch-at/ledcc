#ifndef ANIMATIONPLAYLISTWIDGET_HPP
#define ANIMATIONPLAYLISTWIDGET_HPP

#include <QListWidget>

class AnimationPlayListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit AnimationPlayListWidget(QWidget *parent = Q_NULLPTR);

signals:
    void updateUi(void);
public Q_SLOTS:
    void clearList(void);
    void newItem(QListWidgetItem *item);
protected:
    virtual void keyPressEvent(QKeyEvent * event);
};

#endif // ANIMATIONPLAYLISTWIDGET_HPP
