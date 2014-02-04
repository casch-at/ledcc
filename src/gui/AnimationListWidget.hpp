#ifndef ANIMATIONLISTWIDGET_HPP
#define ANIMATIONLISTWIDGET_HPP

#include <QListWidget>

class AnimationListWidget : public QListWidget
{
    Q_OBJECT
public:
    AnimationListWidget(QWidget* parent = Q_NULLPTR);
signals:
protected:
    virtual void 	keyPressEvent(QKeyEvent * event);
};

#endif // ANIMATIONLISTWIDGET_HPP
