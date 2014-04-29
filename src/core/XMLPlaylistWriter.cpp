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

#include "XMLPlaylistWriter.hpp"

#include "AnimationItem.hpp"
#include "System.hpp"
// Qt Includes
#include <QXmlStreamWriter>
#include <QDir>

XMLPlaylistWriter::XMLPlaylistWriter()
{
}

int XMLPlaylistWriter::writeAnimationPlaylist(const QList<AnimationItem *> *animationItemList)
{
    System system;
    QFile file(system.getConfigPath() + "animations.xml");

    if (!file.open(QIODevice::WriteOnly)) {  // INFO:: Print error message to user
        return -1;
    } else {
        QXmlStreamWriter *xmlWriter = new QXmlStreamWriter();
        xmlWriter->setAutoFormatting(true);
        xmlWriter->setDevice(&file);
        // Writes a document start with the XML version number
        xmlWriter->writeStartDocument();
        // Writes a start element with name
        xmlWriter->writeStartElement("animations");

        foreach (AnimationItem * animation, *animationItemList) {
            Options *options = const_cast<Options*>(animation->getOptions());

            // Create new StarteElement for the animations
            xmlWriter->writeStartElement("animation");
            xmlWriter->writeAttribute("name", animation->text());

            // Write attributes of the animation
            xmlWriter->writeStartElement("delay");
            xmlWriter->writeAttribute("delay", QString("%1").arg(options->m_delay));
            xmlWriter->writeEndElement();

            xmlWriter->writeStartElement("direction");
            xmlWriter->writeAttribute("direction", QString("%1").arg(options->m_direction));
            xmlWriter->writeEndElement();

            xmlWriter->writeStartElement("invert");
            xmlWriter->writeAttribute("invert", QString("%1").arg(options->m_invert));
            xmlWriter->writeEndElement();

            xmlWriter->writeStartElement("iteration");
            xmlWriter->writeAttribute("iteration", QString("%1").arg(options->m_iteration));
            xmlWriter->writeEndElement();

            xmlWriter->writeStartElement("leds");
            xmlWriter->writeAttribute("leds", QString("%1").arg(options->m_leds));
            xmlWriter->writeEndElement();

            xmlWriter->writeStartElement("speed");
            xmlWriter->writeAttribute("speed", QString("%1").arg(options->m_speed));
            xmlWriter->writeEndElement();

            xmlWriter->writeStartElement("state");
            xmlWriter->writeAttribute("state", QString("%1").arg(options->m_state));
            xmlWriter->writeEndElement();

            xmlWriter->writeStartElement("text");
            xmlWriter->writeAttribute("text", QString(options->m_text));
            xmlWriter->writeEndElement();

            xmlWriter->writeEndElement(); // Write end element "animation"
        }
        xmlWriter->writeEndElement(); // Write end element "animations"

        xmlWriter->writeEndDocument(); // Close everything we are done
        delete xmlWriter;
    }
    return 0;
}



