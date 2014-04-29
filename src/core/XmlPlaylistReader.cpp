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
    if (attributes.hasAttribute("name"))
        animationItem = new AnimationItem(attributes.value("name").toString());

    xmlReader->readNext();

    while (!(xmlReader->tokenType() == QXmlStreamReader::EndElement && xmlReader->name() == "animation")) {
        Options options;

        attributes = xmlReader->attributes();
        if (attributes.hasAttribute("options"))
            animationItem->setAvailableAnimationOptions(attributes.value("options").toString().toInt());

        xmlReader->readNext();
        if (xmlReader->tokenType() == QXmlStreamReader::StartElement) {
            attributes = xmlReader->attributes();
            readAnimationProperties(xmlReader, &attributes, &options);
            xmlReader->readNext();
        }

        animationItem->setOptions(options);
        xmlReader->readNext();
    }
    animationItem->createAnimationTooltipAsRichText();
    return animationItem;
}

/*!
 \brief

 \param xmlReader Pointer to the \a QXmlStreamReader
 \param options Pointer to the animation Options object
 \return int Returns -1 on error otherwise 0
*/
int XmlPlaylistReader::readAnimationProperties(QXmlStreamReader *xmlReader, QXmlStreamAttributes *attributes, Options *options)
{
    qDebug() << "Attributes" << attributes->value("speed").toString();
    qDebug() << "Attributes" << attributes->value("speed").toString().toInt();
    if (attributes->hasAttribute("delay")) {
        options->m_delay = attributes->value("delay").toString().toInt();
    }

    if (xmlReader->name() == "direction") {
        options->m_direction = static_cast<Draw::Direction>(xmlReader->text().toString().toInt());
    }

    if (xmlReader->name() == "invert") {
        options->m_invert = xmlReader->text().toString().toInt() ? true : false;
    }

    if (xmlReader->name() == "iteration") {
        options->m_iteration = xmlReader->text().toString().toInt();
    }

    if (xmlReader->name() == "leds") {
        options->m_leds = xmlReader->text().toString().toInt();
    }

    if (attributes->hasAttribute("speed")) {
        options->m_speed = attributes->value("speed").toString().toInt();
    }

    if (attributes->hasAttribute("state")) {
        options->m_state = static_cast<Draw::BixelState>(attributes->value("state").toString().toInt());
    }

    if (xmlReader->name() == "text") {
        options->m_text = xmlReader->text().toString();
    }
    return 0;
}

void XmlPlaylistReader::cleanUpOnError(QList<AnimationItem*> *animationItemList)
{
    foreach (AnimationItem* item, *animationItemList) {
        delete item;
    }
}

