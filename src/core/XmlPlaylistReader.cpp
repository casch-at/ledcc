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
#include "System.hpp"
#include "AnimationItem.hpp"

// Qt Includes
#include <QXmlStreamReader>
#include <QDir>

#ifdef _DEBUG_
#include <QDebug>
#endif

XmlPlaylistReader::XmlPlaylistReader()
{
}

QList<AnimationItem *> XmlPlaylistReader::readAnimationPlaylist(const QString &xmlPlaylist)
{
    QList<AnimationItem*> animationItemList;
    QFile file(xmlPlaylist);

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
                    AnimationItem *item;
                    if( (item = parseAnimation(xmlReader)) == Q_NULLPTR ) {
                        cleanUpOnError(&animationItemList);
                        return animationItemList;
                    }
                    animationItemList.append(item);
                }
            }
        }
        delete xmlReader;
    }
    return animationItemList;
}

AnimationItem* XmlPlaylistReader::parseAnimation(QXmlStreamReader *xmlReader)
{
    Options options;
    AnimationItem *animationItem;
    QXmlStreamAttributes attributes = xmlReader->attributes();

    if (attributes.hasAttribute("name"))
        animationItem = new AnimationItem(attributes.value("name").toString());

    while (!(xmlReader->tokenType() == QXmlStreamReader::EndElement && xmlReader->name() == "animation")) {
        xmlReader->readNext();
        attributes = xmlReader->attributes();

        if (xmlReader->tokenType() == QXmlStreamReader::StartElement) {
            if(readAnimationProperties(xmlReader, &attributes, &options, animationItem) == ERRNO ){
                delete animationItem;
                animationItem = Q_NULLPTR;
                return animationItem;
            }
        }

    }
    animationItem->setOptions(options);
    animationItem->createAnimationTooltipAsRichText();
    return animationItem;
}

/*!
 \brief

 \param xmlReader Pointer to the \a QXmlStreamReader
 \param options Pointer to the animation Options object
 \return int Returns -1 on error otherwise 0
*/
int XmlPlaylistReader::readAnimationProperties(QXmlStreamReader *xmlReader, QXmlStreamAttributes *attributes,
                                               Options *options, AnimationItem *animationItem)
{
    if (attributes->hasAttribute("options")){
        animationItem->setAvailableAnimationOptions(attributes->value("options").toString().toInt());
    } else if (attributes->hasAttribute("axis")) {
        options->m_delay = static_cast<Draw::Axis>(attributes->value("axis").toString().toInt());
    } else if (attributes->hasAttribute("delay")) {
        options->m_delay = attributes->value("delay").toString().toInt();
    } else if (xmlReader->name() == "direction") {
        options->m_direction = static_cast<Draw::Direction>(attributes->value("direction").toString().toInt());
    } else if (xmlReader->name() == "invert") {
        options->m_invert = static_cast<bool>(attributes->value("invert").toString().toInt());
    } else if (xmlReader->name() == "iteration") {
        options->m_iteration = attributes->value("iteration").toString().toInt();
    } else if (xmlReader->name() == "leds") {
        options->m_leds = attributes->value("leds").toString().toInt();
    } else if (attributes->hasAttribute("speed")) {
        options->m_speed = attributes->value("speed").toString().toInt();
    } else if (attributes->hasAttribute("state")) {
        options->m_state = static_cast<Draw::BixelState>(attributes->value("state").toString().toInt());
    } else if (xmlReader->name() == "text") {
        options->m_text = attributes->value("text").toString();
    } else {
        return -1;
    }
    return 0;
}

void XmlPlaylistReader::cleanUpOnError(QList<AnimationItem*> *animationItemList)
{
    foreach (AnimationItem* item, *animationItemList) {
        delete item;
    }
}

