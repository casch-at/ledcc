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
#include "Config.hpp"

// ThirdParty
#include "alt_key.hpp"
#include "aqp.hpp"


// Qt includes
#include <QMessageBox>
#include <QCloseEvent>
#include <QMenuBar>
#include <QToolBar>
#include <QByteArray>
#include <QEventLoop>
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
    ui->splitter->setStretchFactor(1,2);
    ui->m_animationList->setFocus();
    updateUi(false);
    m_animationHandler = new AnimationHandler(this,this);
    addActions(QList<QAction *>()
               << ui->m_clearAction << ui->m_duplicateAction << ui->m_editAction
               << ui->m_playAction << ui->m_quitAction << ui->m_removeAction
               << ui->m_settingsAction << ui->m_stopAction << ui->m_moveUpAction
               << ui->m_moveDownAction);
    ui->m_animationPlaylist->addActions(QList<QAction*>()
                                        << ui->m_editAction << ui->m_moveUpAction << ui->m_moveDownAction
                                        << ui->m_duplicateAction << ui->m_removeAction << ui->m_clearAction);
    m_open = false;
    readSettings ();
    connectSignals();
    AQP::accelerateActions(actions());
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow(void)
{
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


/*!
 \brief Save the application settings

*/
void MainWindow::saveSettings(void){
    config()->set(Settings::MainWindowGeometrySettings,saveGeometry());
    config()->set(Settings::AnimationOptionPreviewGeometry, ui->m_animationPropertiesPreview->saveGeometry());
    config()->set(Settings::IsAnimationOptionPreviewHidden, ui->m_animationPropertiesPreview->isHidden());
    config()->set(Settings::IsAnimationToolbarHidden, ui->m_animationTB->isHidden());
    config()->set(Settings::IsHelpToolbarHidden, ui->m_helpTB->isHidden());
    config()->set(Settings::IsMainToolbarHidden, ui->m_mainTB->isHidden());
}

/*!
 \brief Restore the saved application settings

*/
void MainWindow::readSettings (void){
    restoreGeometry (config()->get(Settings::MainWindowGeometrySettings).toByteArray ());
    ui->m_animationPropertiesPreview->restoreGeometry( config()->get(Settings::AnimationOptionPreviewGeometry).toByteArray ());
    ui->m_animationPropertiesPreview->setHidden(config()->get(Settings::IsAnimationOptionPreviewHidden).toBool());
    ui->m_animationTB->setHidden(config()->get(Settings::IsAnimationToolbarHidden).toBool());
    ui->m_helpTB->setHidden(config()->get(Settings::IsHelpToolbarHidden).toBool());
    ui->m_mainTB->setHidden(config()->get(Settings::IsMainToolbarHidden).toBool());
}

/**
 * @author  Christian Schwarzgruber
 * @brief   MainWindow::updateUi
 */

/*!
 \brief Gets called when the serial port gets opened or closed and when the animation playlist gets modified.
        Configures the application action buttons according to the boolean value.

 \param portOpen
*/
void MainWindow::updateUi(bool portOpen)
{

    if(portOpen)
    {
        if(!m_open)
        {
            m_open = true;
            ui->m_openClosePortAction->setIcon( QIcon( "://images/disconnect.png"));
            ui->m_openClosePortAction->setToolTip(tr("Disconnect from seriell device  O"));
        }
    }else
    {
        if(m_open)
        {
            m_open = false;
            ui->m_openClosePortAction->setIcon( QIcon( "://images/connect.png"));
            ui->m_openClosePortAction->setToolTip(tr("Connect to seriell device  O"));
        }

    }

    updateAnimationActions();

}

void MainWindow::updateAnimationActions()
{

    if (m_open){
        if( ui->m_animationPlaylist->count() && !ui->m_stopAction->isEnabled()){
            ui->m_playAction->setEnabled(true);
            ui->m_stopAction->setDisabled(true);
        }
        else{
            ui->m_playAction->setDisabled(true);
        }
    } else {
        ui->m_playAction->setDisabled(true);
        ui->m_stopAction->setDisabled(true);
    }

    if(ui->m_animationPlaylist->count())
        foreach (QAction *a, ui->m_animationPlaylist->actions())
            a->setEnabled(true);
    else
        foreach (QAction *a, ui->m_animationPlaylist->actions())
            a->setDisabled(true);
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
    connect( ui->m_animationList , &AnimationListWidget::showPropertiePreview , ui->m_animationPropertiesPreview , &AnimationPropertiesPreview::createPropertiePreview);
    connect( ui->m_animationPlaylist, &AnimationPlayListWidget::contantChanged , this, &MainWindow::updateAnimationActions);
    //    connect( ui->m_animationPlaylist, &AnimationPlayListWidget::displayAnimationOptions, ui->animationAdjustGB, &AnimationOptions::displayAnimationOptions);
    connect( ui->m_animationPlaylist, &AnimationPlayListWidget::showPropertiePreview, ui->m_animationPropertiesPreview, &AnimationPropertiesPreview::createPropertiePreview);

    // Animation Playlist action
    connect( ui->m_clearAction, &QAction::triggered, ui->m_animationPlaylist, &AnimationPlayListWidget::clearList);
    connect( ui->m_duplicateAction, &QAction::triggered, ui->m_animationPlaylist, &AnimationPlayListWidget::duplicateItems);
    connect( ui->m_removeAction, &QAction::triggered, ui->m_animationPlaylist, &AnimationPlayListWidget::removeItems);
    connect( ui->m_moveUpAction, &QAction::triggered, ui->m_animationPlaylist, &AnimationPlayListWidget::moveItemsUpDown);
    connect( ui->m_moveDownAction, &QAction::triggered, ui->m_animationPlaylist, &AnimationPlayListWidget::moveItemsUpDown);
    connect( ui->m_editAction, &QAction::triggered, ui->m_animationPlaylist, &AnimationPlayListWidget::editItem);
    connect( ui->m_playAction, &QAction::triggered, m_animationHandler, &AnimationHandler::playAnimations);


    connect( m_animationHandler->getSender(), &Sender::portOpenChanged, this, &MainWindow::updateUi);
    connect( m_animationHandler, &AnimationHandler::updateUi, this, &MainWindow::updateUi);
}

/**
 * @brief About the Application
 *
 */
void MainWindow::about()
{
    QMessageBox::about(this, tr("About 3D-LED Cube"),
                       tr("<h2> 3D-LED Cube v%1</h2>"
                          "<p> Copyright &copy; 2014 Christian Schwarzgruber"
                          "<p>The <b>3D-LED Cube</b> program was part of my thesis."
                          "This program lets you rearange the animation in the order you like it, you can even adjust speed,"
                          "delay, iterations and much more.").arg(LEDCC_VERSION));
}
