#ifndef ANIMATIONLISTWIDGET_HPP
#define ANIMATIONLISTWIDGET_HPP

#include <QListWidget>
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
    void insertAnimation(const QString &animation);
private:
};

#endif // ANIMATIONLISTWIDGET_HPP
