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

#include "XmlPlaylistReader.hpp"

#include "AnimationItem.hpp"
#include "System.hpp"
// Qt Includes
#include <QXmlStreamReader>
#include <QDir>

#ifdef _DEBUG_
#include <QDebug>
#endif

XmlPlaylistReader::XmlPlaylistReader()
{
}

QList<AnimationItem *> XmlPlaylistReader::readAnimationPlaylist()
{
    QList<AnimationItem*> animationItemList;
    System system;

    QFile file(system.getConfigPath() + "animations.xml");

    if (!file.open(QIODevice::ReadOnly)) {
        return animationItemList;
    } else {
        QXmlStreamReader *xmlReader = new QXmlStreamReader(&file);
        while(!xmlReader->atEnd() && !xmlReader->hasError()){
            QXmlStreamReader::TokenType token = xmlReader->readNext();

            if (token == QXmlStreamReader::StartElement) {
                if (xmlReader->name() == "animations") {
                    continue;
                }

                if (xmlReader->name() == "animation") {
                    animationItemList.append(parseAnimation(xmlReader));
                }
            }
        }
    }
    return animationItemList;
}

AnimationItem* XmlPlaylistReader::parseAnimation(QXmlStreamReader *xmlReader)
{
    QXmlStreamAttributes attributes = xmlReader->attributes();

    AnimationItem *animationItem;
    if (attributes.hasAttribute("name")) {
        animationItem = new AnimationItem(attributes.value("name").toString());
    }
    xmlReader->readNext();

    while (!(xmlReader->tokenType() == QXmlStreamReader::EndElement && xmlReader->name() == "animation")) {
        if (xmlReader->tokenType() == QXmlStreamReader::StartElement) {
            Options options;
            if (xmlReader->name() == "delay") {
                readAnimationProperties(xmlReader, &options);
            }
        }
    }

//    AnimationItem animation = new AnimationItem()

    return Q_NULLPTR;

}

int XmlPlaylistReader::readAnimationProperties(QXmlStreamReader *xmlReader, Options *options)
{

}

void XmlPlaylistReader::cleanUpOnError(QList<AnimationItem*> *animationItemList)
{
    foreach (AnimationItem* item, *animationItemList) {
        delete item;
    }
}

