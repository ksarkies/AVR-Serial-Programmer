/*
Title:    Atmel Microcontroller Serial Port FLASH loader. T2313 Lock/Fuse bits
*/

/****************************************************************************
 *   Copyright (C) 2007 by Ken Sarkies                                      *
 *   ksarkies@trinity.asn.au                                                *
 *                                                                          *
 *   This file is part of avr-serial-prog                                   *
 *                                                                          *
 *   avr-serial-prog is free software; you can redistribute it and/or modify*
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   avr-serial-prog is distributed in the hope that it will be useful,     *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with avr-serial-prog if not, write to the                        *
 *   Free Software Foundation, Inc.,                                        *
 *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.              *
 ***************************************************************************/

#ifndef T2313_DIALOG_H
#define T2313_DIALOG_H
#define _TTY_POSIX_         // Need to tell qextserialport we are in POSIX

#include "ui/ui_t261Dialog.h"
#include "serialport.h"
#include <QDialog>
#include <QCloseEvent>

//-----------------------------------------------------------------------------
/** @brief AVR Serial Programmer m48 Type Lock/Fuse bit Dialogue

This class provides a dialogue window
*/

class T2313Dialog : public QDialog
{
    Q_OBJECT
public:
    T2313Dialog(SerialPort*, QWidget* parent = 0);
    ~T2313Dialog();
    void setDefaults(uchar l, uchar e, uchar h, uchar f);
private slots:
    void on_closeButton_clicked();
    void on_memoryLockBox_currentIndexChanged(int memoryLock);
    void on_lockWriteButton_clicked();
    void on_extFuseWriteButton_clicked();
    void on_highFuseWriteButton_clicked();
    void on_fuseWriteButton_clicked();
private:
    SerialPort* port;           //!< Serial port object pointer
    uchar lockBitsOriginal;
    uchar extFuseBitsOriginal;
    uchar highFuseBitsOriginal;
    uchar fuseBitsOriginal;
    char inBuffer[10];
// User Interface object
    Ui::T261Dialog t261DialogFormUi;
};

#endif
