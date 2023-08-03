#ifndef MY_DEVICE_H
#define MY_DEVICE_H

#include "device/device_dht.h"
#include "device/device_lcd.h"
#include "device/device_pir.h"
#include "device/device_relay.h"
#include "device/device_IR.h"

class MyDevice
{
    private:
        DeviceDHT dht;
        DeviceLCD lcd;
        DevicePIR pir;
        DeviceRelay relay;
        DeviceIR ir;
    public:
        void SetUp()
        {
            dht.SetUp();
            lcd.SetUp();
            pir.SetUp();
            relay.SetUp();
            ir.SetUp();
        }       
};

#endif