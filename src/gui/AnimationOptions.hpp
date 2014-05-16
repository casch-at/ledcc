/**
 * Copyright (C) 2014  Christian Schwarzgruber <christiandev9@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef ANIMATIONOPTIONS_HPP
#define ANIMATIONOPTIONS_HPP
#include "Global.hpp"
#include "AnimationItem.hpp"
#include <QtWidgets/QDialog>

class QTimer;

namespace Ui {
    class AnimationOptions;
    }


/*!
 \brief Dialog to adjust the animation properties
*/
class AnimationOptions : public QDialog
{
    Q_OBJECT

public:
    explicit AnimationOptions(QList<AnimationItem *> &itemsList, QWidget *parent = 0);
    ~AnimationOptions();

    /*!
    *  All possibly animation arguments.
    *  The static variable \a TOTAL_ARGUMENTS holds the sum of available options, must be updated when an enum is added
    */

public Q_SLOTS:
Q_SIGNALS:
    void optionsReady(const AnimationItem::Options &animationOptions);
    void applyNewAnimationArguments(AnimationItem *item);
protected:
    void changeEvent(QEvent *e);
private Q_SLOTS:
    void optionsNextAnimation();
    void optionsPrevAnimation();
    void applyAnimationOptions();
    void cancel();
    void ok();
    void updateUi();
    void compareOldNewAnimationOptions();
    void compareOldNewAnimationOptions(int index);
private:
    void hideShowWidgetsDisplayOptions();
    void shouldeApplyNewOptions();
    int okToContinue();
    void connectAll();
    void disconnectAll();

private:
    Ui::AnimationOptions *ui;
    int m_animationAt;
    QList<AnimationItem*> m_itemList;
    AnimationItem *m_animationToUpdate;
    Q_DISABLE_COPY(AnimationOptions)
};

#endif // ANIMATIONOPTIONSGROUPBOX_HPP
