#ifndef ANIMATIONOPTIONSGROUPBOX_HPP
#define ANIMATIONOPTIONSGROUPBOX_HPP

#include <QGroupBox>
#include "Global.hpp"

namespace Ui {
    class AnimationOptionsGroupBox;
    }

class AnimationOptionsGroupBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit AnimationOptionsGroupBox(QWidget *parent = 0);
    ~AnimationOptionsGroupBox();
    AnimationStruct *getAnimationSettings(void);
public Q_SLOTS:
    void enableProperty(const QString &animation);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::AnimationOptionsGroupBox *ui;
    AnimationStruct animationStruct;
};

#endif // ANIMATIONOPTIONSGROUPBOX_HPP
