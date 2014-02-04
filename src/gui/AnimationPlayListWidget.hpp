#ifndef ANIMATIONPLAYLISTWIDGET_HPP
#define ANIMATIONPLAYLISTWIDGET_HPP

#include <QListWidget>

class QShortcut;
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
private:
    QShortcut *shortCutSA;
};

#endif // ANIMATIONPLAYLISTWIDGET_HPP
