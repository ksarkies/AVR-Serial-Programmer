/*
@brief                        M48 Type Lock/Fuse bits

@details Open a dialogue window with lock and fuse bits available for editing and
programming. This applies to the ATMega48.

*/
/****************************************************************************
 *   Copyright (C) 2007 by Ken Sarkies                                      *
 *   ksarkies@trinity.asn.au                                                *
 *                                                                          *
 *   This file is part of serial-programmer                                 *
 *                                                                          *
 *   serial-programmer is free software; you can redistribute it and/or     *
 *   modify it under the terms of the GNU General Public License as         *
 *   published bythe Free Software Foundation; either version 2 of the      *
 *   License, or (at your option) any later version.                        *
 *                                                                          *
 *   serial-programmer is distributed in the hope that it will be useful,   *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with serial-programmer if not, write to the                      *
 *   Free Software Foundation, Inc.,                                        *
 *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.              *
 ***************************************************************************/

#include "m48Dialog.h"
#include <QSerialPort>
#include <QString>
#include <QLabel>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include <QBasicTimer>
#include <cstdlib>
#include <iostream>

//-----------------------------------------------------------------------------
/** Constructor

@param p Serial Port object pointer
@param parent Parent widget.
*/

M48Dialog::M48Dialog(QSerialPort* p, QWidget* parent) : QDialog(parent)
{
    port = p;
// Build the User Interface display from the Ui class in ui_mainwindowform.h
    m48DialogFormUi.setupUi(this);
}

M48Dialog::~M48Dialog()
{
//    port->close();
}

//-----------------------------------------------------------------------------
/** @brief Setup the widgets to a default state

The default values for the various lock/fuse bits are set and the widgets set to indicate these.
*/

void M48Dialog::setDefaults(uchar l, uchar e, uchar h, uchar f)
{
// Set the lock bit widgets to represent the current status.
    lockBitsOriginal = l;
    uint memoryLockOriginal = (lockBitsOriginal&0x03);
    if (memoryLockOriginal == 0) m48DialogFormUi.memoryLockBox->setCurrentIndex(2);
    else if (memoryLockOriginal == 2) m48DialogFormUi.memoryLockBox->setCurrentIndex(1);
    else if (memoryLockOriginal == 3) m48DialogFormUi.memoryLockBox->setCurrentIndex(0);
    uint appLockOriginal = ((lockBitsOriginal >> 2)&0x03);
    if (appLockOriginal == 0) m48DialogFormUi.appLockBox->setCurrentIndex(2);
    else if (appLockOriginal == 1) m48DialogFormUi.appLockBox->setCurrentIndex(3);
    else if (appLockOriginal == 2) m48DialogFormUi.appLockBox->setCurrentIndex(1);
    else if (appLockOriginal == 3) m48DialogFormUi.appLockBox->setCurrentIndex(0);
    uint bootLockOriginal = ((lockBitsOriginal >> 4)&0x03);
    if (bootLockOriginal == 0) m48DialogFormUi.bootLockBox->setCurrentIndex(2);
    else if (bootLockOriginal == 1) m48DialogFormUi.bootLockBox->setCurrentIndex(3);
    else if (bootLockOriginal == 2) m48DialogFormUi.bootLockBox->setCurrentIndex(1);
    else if (bootLockOriginal == 3) m48DialogFormUi.bootLockBox->setCurrentIndex(0);
// Set the extended fuse bit widgets to represent the current status.
    extFuseBitsOriginal = e;
    if ((extFuseBitsOriginal&0x01)>0)
        m48DialogFormUi.selfProgCheck->setChecked(false);
    else
        m48DialogFormUi.selfProgCheck->setChecked(true);
// Set the high fuse bit widgets to represent the current status.
    highFuseBitsOriginal = h;
    m48DialogFormUi.brownoutBox->setCurrentIndex(0x03-(highFuseBitsOriginal&0x03));
    if ((highFuseBitsOriginal&0x08)>0)
        m48DialogFormUi.preserveEEPROMBox->setChecked(false);
    else
        m48DialogFormUi.preserveEEPROMBox->setChecked(true);
    if ((highFuseBitsOriginal&0x10)>0)
        m48DialogFormUi.watchdogOnBox->setChecked(false);
    else
        m48DialogFormUi.watchdogOnBox->setChecked(true);
    if ((highFuseBitsOriginal&0x20)>0)
        m48DialogFormUi.enableSerialBox->setChecked(false);
    else
        m48DialogFormUi.enableSerialBox->setChecked(true);
    if ((highFuseBitsOriginal&0x40)>0)
        m48DialogFormUi.debugWIREBox->setChecked(false);
    else
        m48DialogFormUi.debugWIREBox->setChecked(true);
    if ((highFuseBitsOriginal&0x80)>0)
        m48DialogFormUi.resetDisableBox->setChecked(false);
    else
        m48DialogFormUi.resetDisableBox->setChecked(true);
// Set the fuse bit widgets to represent the current status.
    fuseBitsOriginal = f;
    if ((fuseBitsOriginal&0x01)>0)
        m48DialogFormUi.clockSourceBox->setChecked(true);
    else
        m48DialogFormUi.clockSourceBox->setChecked(false);
    if ((fuseBitsOriginal&0x02)>0)
        m48DialogFormUi.clockSourceBox_2->setChecked(true);
    else
        m48DialogFormUi.clockSourceBox_2->setChecked(false);
    if ((fuseBitsOriginal&0x04)>0)
        m48DialogFormUi.clockSourceBox_3->setChecked(true);
    else
        m48DialogFormUi.clockSourceBox_3->setChecked(false);
    if ((fuseBitsOriginal&0x08)>0)
        m48DialogFormUi.clockSourceBox_4->setChecked(true);
    else
        m48DialogFormUi.clockSourceBox_4->setChecked(false);
    if ((fuseBitsOriginal&0x10)>0)
        m48DialogFormUi.startupTimeBox->setChecked(true);
    else
        m48DialogFormUi.startupTimeBox->setChecked(false);
    if ((fuseBitsOriginal&0x20)>0)
        m48DialogFormUi.startupTimeBox_2->setChecked(true);
    else
        m48DialogFormUi.startupTimeBox_2->setChecked(false);
    if ((fuseBitsOriginal&0x40)>0)
        m48DialogFormUi.clockOutBox->setChecked(false);
    else
        m48DialogFormUi.clockOutBox->setChecked(true);
    if ((fuseBitsOriginal&0x80)>0)
        m48DialogFormUi.clockDivide8Box->setChecked(false);
    else
        m48DialogFormUi.clockDivide8Box->setChecked(true);
}

//-----------------------------------------------------------------------------
/** @brief Close when button is activated.

No further action is taken, and the device may remain in the bootloader.
*/

void M48Dialog::on_closeButton_clicked()
{
    reject();
}

//-----------------------------------------------------------------------------
/** @brief Do some checks and write the selected lock bits.

Interpret the selection and build the lock bits.
The first two bits control memory lock (only three settings).
The next two determine application memory locking.
The next two determine boot memory locking.
The last two are reserved and unprogrammed (set to 1's).
*/

void M48Dialog::on_lockWriteButton_clicked()
{
    uchar lockBits = 0;                             // All read/write locked including boot lock bits
    int memoryLock = m48DialogFormUi.memoryLockBox->currentIndex();
    if (memoryLock == 0) lockBits = 3;              // Set to no memory locks
    else if (memoryLock == 1) lockBits = 2;         // Disable Write to FLASH, EEPROM, Fuses
    if (memoryLock < 2)
    {
        int appLock = m48DialogFormUi.appLockBox->currentIndex();
        uchar appLockBits = 0;
        if (appLock == 0) appLockBits = 0x0C;
        else if (appLock == 1) appLockBits = 0x08;
        else if (appLock == 3) appLockBits = 0x04;
        int bootLock = m48DialogFormUi.bootLockBox->currentIndex();
        uchar bootLockBits = 0;
        if (bootLock == 0) bootLockBits = 0x30;
        else if (bootLock == 1) bootLockBits = 0x20;
        else if (bootLock == 3) bootLockBits = 0x10;
        lockBits |= (bootLockBits | appLockBits | 0xC0);
    }
    else lockBits = 0xFC;                           // Set all higher bits to 1's
    if (lockBits != lockBitsOriginal)
    {
        port->putChar('l');                         // Issue a lock byte write request
        port->putChar(lockBits);
        port->read(inBuffer,1);                     // Pull in and discard response
    }
}

//-----------------------------------------------------------------------------
/** @brief Act on changes to the memoryLock ComboBox

Check the new setting and disable or enable the other boxes accordingly.
A QFrame can be disabled in its entirety.
*/

void M48Dialog::on_memoryLockBox_currentIndexChanged(int memoryLock)
{
    if (memoryLock == 2)
    {
        m48DialogFormUi.appLockBox->setEnabled(false);  // Disable as these lock bits cannot be changed.
        m48DialogFormUi.bootLockBox->setEnabled(false);
    }
    else
    {
        m48DialogFormUi.appLockBox->setEnabled(true);
        m48DialogFormUi.bootLockBox->setEnabled(true);
    }
    if (memoryLock != 0)
    {
        m48DialogFormUi.extFuseFrame->setEnabled(false);
        m48DialogFormUi.highFuseFrame->setEnabled(false);
        m48DialogFormUi.fuseFrame->setEnabled(false);
    }
    else
    {
        m48DialogFormUi.extFuseFrame->setEnabled(true);
        m48DialogFormUi.highFuseFrame->setEnabled(true);
        m48DialogFormUi.fuseFrame->setEnabled(true);
    }
}

//-----------------------------------------------------------------------------
/** @brief Do some checks and write the selected extended fuse bits.

Interpret the selection and build the extended fuse bits.
The first bit sets how the boot vector works, high selects 0000 and low sets the bootloader.
The next two determine bootloader memory size.
*/

void M48Dialog::on_extFuseWriteButton_clicked()
{
    uchar extFuseBits = 0;
    if (!m48DialogFormUi.selfProgCheck->isChecked()) extFuseBits+=1;
    if (extFuseBits != extFuseBitsOriginal)
    {
        port->putChar('q');                         // Issue a extended fuse byte write request
        port->putChar(extFuseBits);
        int numBytes = 0;
        while(numBytes <= 0) numBytes = port->bytesAvailable();
        port->read(inBuffer,1);                     // Pull in and discard response
    }
}

//-----------------------------------------------------------------------------
/** @brief Do some checks and write the selected high fuse bits.

(note the programmed state is 0, which is boolean false)
Interpret the selection and build the high fuse bits.
The first three bits set the brownout voltage level threshold: 0=disabled,l 1=1.8V, 2=2.7V, 3=4.3V
2. The third bit of brownout threshold is reserved (always 1).
3. EEPROM programming is preserved through a chip erase.
4. Watchdog timer is always on.
5. Enable serial programming and data download.
6. Enable debugWIRE.
7. Disable external reset.
*/

void M48Dialog::on_highFuseWriteButton_clicked()
{
    int bodLevel = m48DialogFormUi.brownoutBox->currentIndex();
    uchar highFuseBits = 0x07-bodLevel;                 // Compute BOD Level settings (3-bodlevel+bit3)
    if (!m48DialogFormUi.preserveEEPROMBox->isChecked()) highFuseBits+=0x08;
    if (!m48DialogFormUi.watchdogOnBox->isChecked()) highFuseBits+=0x10;
    if (!m48DialogFormUi.enableSerialBox->isChecked()) highFuseBits+=0x20;
    if (!m48DialogFormUi.debugWIREBox->isChecked()) highFuseBits+=0x40;
    if (!m48DialogFormUi.resetDisableBox->isChecked()) highFuseBits+=0x80;
    if (highFuseBits != highFuseBitsOriginal)
    {
        port->putChar('n');                         // Issue a high fuse byte write request
        port->putChar(highFuseBits);
        port->read(inBuffer,1);                     // Pull in and discard response
    }
}

//-----------------------------------------------------------------------------
/** @brief Do some checks and write the selected fuse bits.

(note the programmed state is 0, which is boolean false)
Interpret the selection and build the high fuse bits.
The first four bits set the clock source selection. This is complex so the user must determine this.
The next two bits determine the startup time selection, which vary according to the clock source.
6. Clock is output on port PB0.
7. Divide the clock by 8.
*/

void M48Dialog::on_fuseWriteButton_clicked()
{
    uint fuseBits = 0;
    if (m48DialogFormUi.clockSourceBox->isChecked()) fuseBits+=0x01;
    if (m48DialogFormUi.clockSourceBox_2->isChecked()) fuseBits+=0x02;
    if (m48DialogFormUi.clockSourceBox_3->isChecked()) fuseBits+=0x04;
    if (m48DialogFormUi.clockSourceBox_4->isChecked()) fuseBits+=0x08;
    if (m48DialogFormUi.startupTimeBox->isChecked()) fuseBits+=0x10;
    if (m48DialogFormUi.startupTimeBox_2->isChecked()) fuseBits+=0x20;
    if (!m48DialogFormUi.clockOutBox->isChecked()) fuseBits+=0x40;
    if (!m48DialogFormUi.clockDivide8Box->isChecked()) fuseBits+=0x80;
    if (fuseBits != fuseBitsOriginal)
    {
        port->putChar('f');                         // Issue a fuse byte write request
        port->putChar(fuseBits);
        port->read(inBuffer,1);                     // Pull in and discard response
    }
}
//-----------------------------------------------------------------------------

