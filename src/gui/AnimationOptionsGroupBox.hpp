#ifndef ANIMATIONOPTIONSGROUPBOX_HPP
#define ANIMATIONOPTIONSGROUPBOX_HPP

#include <QGroupBox>
#include "Global.hpp"
#include "animations/Draw.hpp"
namespace Ui {
    class AnimationOptionsGroupBox;
    }

class AnimationOptionsGroupBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit AnimationOptionsGroupBox(QWidget *parent = 0);
    ~AnimationOptionsGroupBox();
    Draw::AnimationOptions *getAnimationSettings(void);
Q_SIGNALS:
    void optionsReady(const Draw::AnimationOptions *animationOptions);
public Q_SLOTS:
    void enableProperty(const uint8_t &animation);
protected:
    void changeEvent(QEvent *e);

private slots:
    void on_applyPushB_clicked();

private:
    Ui::AnimationOptionsGroupBox *ui;
    Draw::AnimationOptions animationOptions;
};

#endif // ANIMATIONOPTIONSGROUPBOX_HPP
