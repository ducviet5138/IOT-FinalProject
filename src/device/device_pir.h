#ifndef DEVICE_PIR_H
#define DEVICE_PIR_H

#include <Arduino.h>

class DevicePIR
{
    private:
        const int pir_pin = 23;

    public:
        void SetUp()
        {
            pinMode(pir_pin, INPUT);
        }

        bool IsMotion()
        {
            return digitalRead(pir_pin);
        }
};

#endif
