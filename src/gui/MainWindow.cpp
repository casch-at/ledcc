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
#include "ui_MainWindow.h"

// Application includes
#include "AnimationHandler.hpp"
#include "AnimationOptions.hpp"
#include "Sender.hpp"
// ThirdParty
#include "alt_key.hpp"
#include "aqp.hpp"


// Qt includes
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>
#include <QMenuBar>
#include <QToolBar>
#include <QByteArray>
#include <QEventLoop>
#include <QSerialPort>
#include <QShortcut>

#ifdef _DEBUG_
#include <QDebug>
#endif

// Namespaces
using namespace std;
/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_focusAnimationList(new  QShortcut(QKeySequence(tr("Alt+1")),this)),
    m_focusAnimationPlaylist(new  QShortcut(QKeySequence(tr("Alt+2")),this)),
    m_scSellectAll(new  QShortcut(QKeySequence(tr("Ctrl+A")),this))

{
    ui->setupUi(this); // Ui must be first created befor accessing the elements

    m_animationHandler = new AnimationHandler(this,this);
    readSettings ();
    connectSignals();
    ui->splitter->setStretchFactor(1,2);
    ui->m_animationList->setFocus();
//    AQP::accelerateWidget (this);  //Give each button a accelerater

    updateUi(false);
    m_animationHandler->setAction(ui->m_playAction,1);
    m_animationHandler->setAction(ui->m_stopAction,0);
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow(void)
{
    qDebug("Hello MainWindow destructor");
    delete m_animationHandler;
    delete ui;
}

/**
 * @brief MainWindow::closeEvent
 * @param event
 */
void MainWindow::closeEvent( QCloseEvent *event ) {
    if ( okToContinue() ) {
        QList<QDialog *> allDialogs = findChildren<QDialog *>();
        allDialogs = m_animationHandler->findChildren<QDialog *>();
        for(int i = 0; i < allDialogs.size(); ++i) {
            allDialogs.at(i)->close();
        }
        saveSettings ();
        event->accept();
    }
    else
        event->ignore();
}

/**
 * @brief MainWindow::okToContinue
 * @return boolean
 */
bool MainWindow::okToContinue(void)
{ //Check if window is not modified
    if(isWindowModified ())
    {
        int r = QMessageBox::warning( this, tr( "3D-LED Cube" ),
                                      tr( "Do you really want to exit the programm?" ),
                                      QMessageBox::Yes | QMessageBox::Default,
                                      QMessageBox::Cancel | QMessageBox::Escape );

        if ( r == QMessageBox::Yes )
            return true;
        else if ( r == QMessageBox::Cancel )
            return false;
        return true;
    }
    return true;
}

/**
 * @brief MainWindow::resizeEvent
 * @param e
 */
void MainWindow::resizeEvent(QResizeEvent *e)
{
    QMainWindow::resizeEvent(e);
}

/**
 * @brief MainWindow::saveSettings
 */
void MainWindow::saveSettings(void){  //Save geometry of application
    QSettings settings("ledcc","3D-LED Cube");
    settings.setValue (Settings::SMainWindowGeometrySettings,saveGeometry ());
}

/**
 * @brief MainWindow::readSettings
 */
void MainWindow::readSettings (void){ //Load geometry of application
    QSettings settings("ledcc","3D-LED Cube");
    restoreGeometry (settings.value (Settings::SMainWindowGeometrySettings).toByteArray ());
}

/**
 * @author  Christian Schwarzgruber
 * @brief   MainWindow::updateUi gets called when the serial port gets opened or closed
 *          and when the animation playlist gets modified.
 *          Disables/Enables the playbutton and changes the Seriel connect button appropriated
 */
void MainWindow::updateUi(bool portOpen)
{
    int inPlayList = ui->m_animationPlaylist->count();

    if(portOpen)
    {
        if(ui->m_openClosePortAction->text() == "Open port")
        {
            ui->m_openClosePortAction->setText(tr("Close port"));
            ui->m_openClosePortAction->setIcon( QIcon( "://images/disconnect.png"));
            ui->m_openClosePortAction->setToolTip(tr("Disconnect from seriell device  O"));
        }
        if( inPlayList && !ui->m_stopAction->isEnabled()){
            ui->m_playAction->setEnabled(true);
            ui->m_stopAction->setDisabled(true);
        }
        else{
            ui->m_playAction->setDisabled(true);
        }
    }else
    {
        if(ui->m_openClosePortAction->text() == "Close port")
        {
            ui->m_openClosePortAction->setText(tr("Open port"));
            ui->m_openClosePortAction->setIcon( QIcon( "://images/connect.png"));
            ui->m_openClosePortAction->setToolTip(tr("Connect to seriell device  O"));
        }
        ui->m_playAction->setDisabled(true);
        ui->m_stopAction->setDisabled(true);
    }

    if(inPlayList){
        ui->m_editAction->setEnabled(true);
        ui->m_clearAction->setEnabled(true);
        ui->m_removeAction->setEnabled(true);
        ui->m_moveDownAction->setEnabled(true);
        ui->m_moveUpAction->setEnabled(true);
        ui->m_duplicateAction->setEnabled(true);
        ui->m_clearAction->setEnabled(true);
    } else {
        ui->m_editAction->setDisabled(true);
        ui->m_clearAction->setDisabled(true);
        ui->m_removeAction->setDisabled(true);
        ui->m_moveDownAction->setDisabled(true);
        ui->m_moveUpAction->setDisabled(true);
        ui->m_duplicateAction->setDisabled(true);
        ui->m_clearAction->setDisabled(true);
        ui->m_clearAction->setDisabled(true);
    }
}






/**
 * @brief Create connections
 *
 */
void MainWindow::connectSignals(void)
{
    // Overall connections
    connect( ui->m_openClosePortAction, &QAction::triggered, m_animationHandler ,&AnimationHandler::openCloseSerialPort);
    connect( ui->m_quitAction, &QAction::triggered, this,&MainWindow::close);
    connect( ui->m_aboutAction, &QAction::triggered,this,&MainWindow::about);
    connect( ui->m_settingsAction, &QAction::triggered,m_animationHandler->m_settingsDialog,&QWidget::show);
    connect( ui->m_stopAction, &QAction::triggered , m_animationHandler , &AnimationHandler::stopThreads);
    connect( ui->m_aboutQt, &QAction::triggered, this, &QApplication::aboutQt);

    // ListWidgets shortcuts
    connect( m_focusAnimationList, &QShortcut::activated, ui->m_animationList, &ListWidget::focus);
    connect( m_focusAnimationPlaylist, &QShortcut::activated, ui->m_animationPlaylist, &ListWidget::focus);
    connect( m_scSellectAll, &QShortcut::activated, ui->m_animationList, &ListWidget::selectAllItems);
    connect( m_scSellectAll, &QShortcut::activated, ui->m_animationPlaylist, &ListWidget::selectAllItems);

    // ListWidget interconnections
    connect( ui->m_animationList , &AnimationListWidget::addToPlaylist , ui->m_animationPlaylist , &AnimationPlayListWidget::newItem);
    connect( ui->m_animationList , &AnimationListWidget::showPropertiePreview , ui->animationPropertiesPreview , &AnimationPropertiesPreview::createPropertiePreview);
    connect( ui->m_animationPlaylist, &AnimationPlayListWidget::updateUi , this, &MainWindow::updateUi);
//    connect( ui->m_animationPlaylist, &AnimationPlayListWidget::displayAnimationOptions, ui->animationAdjustGB, &AnimationOptions::displayAnimationOptions);
    connect( ui->m_animationPlaylist, &AnimationPlayListWidget::showPropertiePreview, ui->animationPropertiesPreview, &AnimationPropertiesPreview::createPropertiePreview);

    // Animation Playlist action
    connect( ui->m_clearAction, &QAction::triggered, ui->m_animationPlaylist, &AnimationPlayListWidget::clearList);
    connect( ui->m_duplicateAction, &QAction::triggered, ui->m_animationPlaylist, &AnimationPlayListWidget::duplicateItems);
    connect( ui->m_removeAction, &QAction::triggered, ui->m_animationPlaylist, &AnimationPlayListWidget::removeItems);
    connect( ui->m_moveUpAction, &QAction::triggered, ui->m_animationPlaylist, &AnimationPlayListWidget::moveItemsUpDown);
    connect( ui->m_moveDownAction, &QAction::triggered, ui->m_animationPlaylist, &AnimationPlayListWidget::moveItemsUpDown);
    connect( ui->m_editAction, &QAction::triggered, ui->m_animationPlaylist, &AnimationPlayListWidget::editItem);
    connect( m_animationHandler->getSender(), &Sender::portOpenChanged, this, &MainWindow::updateUi);
    ui->m_animationPlaylist->addActions(QList<QAction*>()
                                        << ui->m_playAction << ui->m_stopAction << ui->m_editAction
                                        << ui->m_moveUpAction << ui->m_moveDownAction << ui->m_duplicateAction
                                        << ui->m_removeAction << ui->m_clearAction);

}

/**
 * @brief About the Application
 *
 */
void MainWindow::about()
{
    QMessageBox::about(this, tr("About 3D-LED Cube"),
                       tr("<h2> 3D-LED Cube v0.1</h2>"
                          "<p> Copyright &copy; 2014 Christian Schwarzgruber"
                          "<p>The <b>3D-LED Cube</b> program was part of my thesis."
                          "This program lets you rearange the animation in the order you like it, you can even adjust speed,"
                          "delay, iterations and much more."));
}
