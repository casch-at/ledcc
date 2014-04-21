/*************************************************************
 * 	File Name       : main.cpp
 *************************************************************
 *  Created on	 : Jan 30, 2013
 *  Author 		 : Christian Schwarzgruber
 *  Version		 : 1.0
 *************************************************************
 *  Description	 : main file to show MainWindow
 *************************************************************/

#include "MainWindow.hpp"
#include <QApplication>


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
    w.show();
    return a.exec();
}
