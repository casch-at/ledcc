/*************************************************************
 * 	File Name       : adddialog.cpp
 *************************************************************
 *  Created on	 : Jan 30, 2013
 *  Author 		 : Christian Schwarzgruber
 *  Version		 : 1.0
 *************************************************************
 *  Description	 : ADDDIALOG
 *************************************************************/

#include "adddialog.hpp"
#include "ui_adddialog.h"

ADDDIALOG::ADDDIALOG(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ADDDIALOG)
    {
        setCancel (false);
        ui->setupUi(this);
    }

ADDDIALOG::~ADDDIALOG()
    {
        delete ui;
    }

void ADDDIALOG::on_pushButtonAdd_clicked()
    {
        storeText ();
    }

void ADDDIALOG::on_pushButtonCancel_clicked()
    {
        setCancel (true);
        this->close ();
    }

void ADDDIALOG::on_lineEditItem_returnPressed()
    {
        storeText ();
    }

void ADDDIALOG::storeText()
    {
        if( ! ui->lineEditItem->text ().isEmpty () ){
                setText (ui->lineEditItem->text ());
                this->close ();
            }
    }


