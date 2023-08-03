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
        DeviceDHT device_dht;
        DeviceLCD device_lcd;
        DevicePIR device_pir;
        DeviceRelay device_relay;
        DeviceIR device_ir;
};

#endif