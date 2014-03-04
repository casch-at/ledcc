#ifndef ANIMATIONLISTWIDGET_HPP
#define ANIMATIONLISTWIDGET_HPP

#include <QListWidget>
#include "AnimationItem.hpp"
/**
 * @brief Class AnimationListWidget handles all available animation
 *
 *
 */
class AnimationListWidget : public QListWidget
{
    Q_OBJECT
public:
    AnimationListWidget(QWidget* parent = Q_NULLPTR);
Q_SIGNALS:
    void itemsSelected(QList<QListWidgetItem *> selected);
protected:
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void dragMoveEvent(QDragMoveEvent *e);
public Q_SLOTS:
    void selectAllItems(void);
    void insertAnimation(const QString &animation);
private:
};

#endif // ANIMATIONLISTWIDGET_HPP
