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

#include "MainWindow.hpp"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#ifdef _DEBUG_
#include <QDebug>
#endif
/*!
 \brief Main function

 Main function execudes the MainWindow and returns the MainWindow return value, 0 on
 success and a number != 0 for failue.

 \param argc - Holds the number of given arguments by default 1
 \param argv[] - Holds the given parameters by default path to the executable
 \return int - Exit value, 0 Success != 0 Failure
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QTranslator *qtTranslator = new QTranslator(w.parent());
    QTranslator *ledccTranslator = new QTranslator(w.parent());
    qtTranslator->load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(qtTranslator);

#ifdef _DEBUG_
//    if(!ledccTranslator->load("ledcc_" + QLocale::system().name()))
    if(!ledccTranslator->load("ledcc_en" ))
        qDebug("Error loading translation");
#else
    translator->load("ledcc_de", + QLocale::system().name());
#endif
    a.installTranslator(ledccTranslator);
//    ledccTranslator->load("ledcc_" + QLocale::system().name());
    ledccTranslator->load("ledcc_de" );
    a.installTranslator(ledccTranslator);

    w.show();
    return a.exec();
}
