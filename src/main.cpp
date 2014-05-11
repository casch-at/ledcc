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
    QTranslator translator;
    MainWindow w;
    translator.load(QString("ledcc_en"));
    a.installTranslator(&translator);
#ifdef _DEBUG_
    if(!translator.load(QString("ledcc_de")))
        qDebug("Error loading translation");
#else
    translator.load(QString("ledcc_de"));
#endif
    a.installTranslator(&translator);
    w.show();
    return a.exec();
}
