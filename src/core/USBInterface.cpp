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

#include "USBInterface.hpp"

#ifdef _DEBUG_
#include <QDebug>
#endif

USBInterface::USBInterface() :
    m_returnVal(0),
    m_countDev(0)
{
    devices.insert("eStickv2",0xfefe);
    m_returnVal = libusb_init(&ctx);
    if (m_returnVal < 0) {
        qDebug() << "Init Error " << m_returnVal;
    }

    libusb_set_debug(ctx, DEBUG_LEVEL);
    m_countDev = libusb_get_device_list(ctx, &m_devList);
    if (m_countDev < 0) {
        qDebug() << "Get Device Error ";
    }

    for (ssize_t i = 0; i < m_countDev; i++) {
        printDev(m_devList[i]);
    }
}

bool USBInterface::connectUSB()
{

    return true;
}

bool USBInterface::disconnectUSB()
{
    return true;
}

#ifdef _DEBUG_
void USBInterface::printDev(libusb_device *dev)
{
    libusb_device_descriptor desc;
      int r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0) {
            qDebug() <<"failed to get device descriptor";
            return;
        }
        qDebug() <<"Number of possible configurations: "<<static_cast<int>(desc.bNumConfigurations)<<"  ";
        qDebug() <<"Device Class: "<< static_cast<int>(desc.bDeviceClass)<<"  ";
        qDebug() <<"VendorID: "<<desc.idVendor<<"  ";
        qDebug() <<"ProductID: "<<desc.idProduct;
        libusb_config_descriptor *config;
        libusb_get_config_descriptor(dev, 0, &config);
        qDebug() <<"Interfaces: "<<static_cast<int>(config->bNumInterfaces)<<" ||| ";
        const libusb_interface *inter;
        const libusb_interface_descriptor *interdesc;
        const libusb_endpoint_descriptor *epdesc;
        for(int i=0; i<static_cast<int>(config->bNumInterfaces); i++) {
            inter = &config->interface[i];
            qDebug() <<"Number of alternate settings: "<<inter->num_altsetting<<" | ";
            for(int j=0; j<inter->num_altsetting; j++) {
                interdesc = &inter->altsetting[j];
                qDebug() <<"Interface Number: "<<static_cast<int>(interdesc->bInterfaceNumber)<<" | ";
                qDebug() <<"Number of endpoints: "<<static_cast<int>(interdesc->bNumEndpoints)<<" | ";
                for(int k=0; k<static_cast<int>(interdesc->bNumEndpoints); k++) {
                    epdesc = &interdesc->endpoint[k];
                    qDebug() <<"Descriptor Type: "<<static_cast<int>(epdesc->bDescriptorType)<<" | ";
                    qDebug() <<"EP Address: "<<static_cast<int>(epdesc->bEndpointAddress)<<" | ";
                }
            }
            qDebug();
            qDebug();
            qDebug();
        }
        libusb_free_config_descriptor(config);
}
#endif
