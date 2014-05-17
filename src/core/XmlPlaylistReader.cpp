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

/*!
 \brief

 \param xmlPlaylist
 \return QList<AnimationItem *>
*/
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

///*!
// \brief

// \param xmlReader
// \return AnimationItem
//*/
//AnimationItem* XmlPlaylistReader::parseAnimation(QXmlStreamReader *xmlReader)
//{
//    Options options;
//    AnimationItem *animationItem;
//    QXmlStreamAttributes attributes = xmlReader->attributes();

//    if (attributes.hasAttribute("name"))
//        animationItem = new AnimationItem(attributes.value("name").toString());

//    while (!(xmlReader->tokenType() == QXmlStreamReader::EndElement && xmlReader->name() == "animation")) {
//        xmlReader->readNext();
//        attributes = xmlReader->attributes();

//        if (xmlReader->tokenType() == QXmlStreamReader::StartElement) {
//            if(readAnimationProperties(xmlReader, &attributes, &options, animationItem) == ERRNO ){
//                delete animationItem;
//                animationItem = Q_NULLPTR;
//                return animationItem;
//            }
//        }

//    }
//    animationItem->setOptions(options);
//    animationItem->createAnimationTooltipAsRichText();
//    return animationItem;
//}
/*!
 \brief

 \param xmlReader
 \return AnimationItem
*/
AnimationItem* XmlPlaylistReader::parseAnimation(QXmlStreamReader *xmlReader)
{
    AnimationItem::Options options;
    AnimationItem *animationItem = new AnimationItem();
    QXmlStreamAttributes attributes = xmlReader->attributes();

    if(readAnimationProperties(&attributes, &options, animationItem) == ERRNO ){
        delete animationItem;
        animationItem = Q_NULLPTR;
        return animationItem;
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
int XmlPlaylistReader::readAnimationProperties(QXmlStreamAttributes *attributes,
                                               AnimationItem::Options *options, AnimationItem *animationItem)
{
    QXmlStreamAttribute attribute;
    for (int i = 0; i < attributes->count(); i++) {
        attribute = attributes->at(i);
        switch (i)
        {
        case 0:
            if (attribute.name().compare("name")) return ERRNO;
            animationItem->setText(attribute.value().toString());
            break;
        case 1:
            if (attribute.name().compare("options")) return ERRNO;
            animationItem->setAvailableAnimationOptions(attribute.value().toString().toInt());
            break;
        case 2:
            if (attribute.name().compare("axis")) return ERRNO;
            options->m_axis = static_cast<Draw::Axis>(attribute.value().toString().toInt());
            break;
        case 3:
            if (attribute.name().compare("delay")) return ERRNO;
            options->m_delay = attribute.value().toString().toInt();
            break;
        case 4:
            if (attribute.name().compare("direction")) return ERRNO;
            options->m_direction = static_cast<Draw::Direction>(attribute.value().toString().toInt());
            break;
        case 5:
            if (attribute.name().compare("invert")) return ERRNO;
            options->m_invert = static_cast<bool>(attribute.value().toString().toInt());
            break;
        case 6:
            if (attribute.name().compare("iteration")) return ERRNO;
            options->m_iteration = attribute.value().toString().toInt();
            break;
        case 7:
            if (attribute.name().compare("leds")) return ERRNO;
            options->m_leds = attribute.value().toString().toInt();
            break;
        case 8:
            if (attribute.name().compare("speed")) return ERRNO;
            options->m_speed = attribute.value().toString().toInt();
            break;
        case 9:
            if (attribute.name().compare("state")) return ERRNO;
            options->m_state = static_cast<Draw::BixelState>(attribute.value().toString().toInt());
            break;
        case 10:
            if (attribute.name().compare("text")) return ERRNO;
            options->m_text = attribute.value().toString();
            break;
        default:
            return ERRNO;
            break;
        }
    }
    return OK;
}

void XmlPlaylistReader::cleanUpOnError(QList<AnimationItem*> *animationItemList)
{
    foreach (AnimationItem* item, *animationItemList) {
        delete item;
    }
}

