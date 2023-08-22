#ifndef DEVICE_PIR_H
#define DEVICE_PIR_H

class DevicePIR
{
    private:
        const int pir_pin = 23;

    public:
        DevicePIR()
        {
            pinMode(pir_pin, INPUT);
        }

        ~DevicePIR(){}

        bool GetPersonStatus()
        {
            return digitalRead(pir_pin);
        }
};

#endif
