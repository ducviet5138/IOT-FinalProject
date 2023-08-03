#ifndef DEVICE_PIR_H
#define DEVICE_PIR_H

class DevicePIR
{
    private:
        const int pir_pin = 23;
        bool is_motion = false;

    public:
        DevicePIR()
        {
            pinMode(pir_pin, INPUT);
        }

        void Update()
        {
            is_motion = digitalRead(pir_pin);
        }

        bool IsMotion()
        {
            return is_motion;
        }
};

#endif
