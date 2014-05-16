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
#ifndef ANIMATIONS_HPP
#define ANIMATIONS_HPP

#include <QObject>
#include <QHash>

// All available build in animations
#include "Draw.hpp"
#include "Lift.hpp"
#include "Firework.hpp"
#include "AxisNailWall.hpp"
#include "WireBoxCenterShrinkGrow.hpp"
#include "WireBoxCornerShrinkGrow.hpp"
#include "Loadbar.hpp"
#include "RandomFiller.hpp"
#include "RandomSpark.hpp"
#include "RandomSparkFlash.hpp"
#include "RandomZLift.hpp"
#include "Wall.hpp"
#include "Rain.hpp"
#include "StringFly.hpp"
#include "ConwaysGameOfLife.hpp"
#include "Wave.hpp"


class Animation;
class AnimationItem;

class Animations
{
public:
    explicit Animations();
    ~Animations();
    Animation* get(const QString& key);
    Animation* get(const QString& key,const AnimationItem* defaultValue);
    Animation* get(const AnimationItem* defaultValue);
    const QHash<QString, Animation *> *getAll();
    QList<AnimationItem*> const * animationItemDefaultList() const { return &m_animationItemDefaultList; }
    void updateAnimation(const AnimationItem *item);
private:
    void setupAnimationItems(void);
    QHash<QString, Animation*> m_animationHash;
    QList<AnimationItem*> m_animationItemDefaultList;
    Q_DISABLE_COPY(Animations)
};


#endif // ANIMATIONS_HPP
