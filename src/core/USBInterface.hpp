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

#ifndef USBINTERFACE_HPP
#define USBINTERFACE_HPP

#include <libusb-1.0/libusb.h>
#include <QObject>
#include <QHash>

class USBInterface : public QObject
{
    Q_OBJECT
public:
    USBInterface();
    bool connectUSB();
    bool disconnectUSB();
    inline int clearUSB(){
        return libusb_clear_halt(eStickv2_handle,BULK_OUT_END_POINT);
    }

private:
#ifdef _DEBUG_
    void printDev(libusb_device *dev);
#endif
    libusb_device **m_devList;
    libusb_device_handle *eStickv2_handle;
    libusb_context *ctx;
    static const quint8 BULK_OUT_END_POINT = 0x05;
    static const quint8 BULK_IN_END_POINT = 0x82;
    static const quint8 DEBUG_LEVEL = 0x03;
    QHash<QString, quint16> devices;

    int m_returnVal;
    ssize_t m_countDev;
    Q_DISABLE_COPY(USBInterface)
};

#endif // USBINTERFACE_HPP
