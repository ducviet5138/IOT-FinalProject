#ifndef DEVICE_IR_H
#define DEVICE_IR_H
#include "my_map.h"
#include <vector>

using namespace std;

class DeviceIR
{
    private:
        const int ir_pin = 4;
        int signal[2][2];
        bool isOn;
        
    public:
        DeviceIR(vector<vector<int>> userSignal)
        {
            for (int i = 0; i < 2; i++)
            {
                signal[i][0] = userSignal[i][0];
                signal[i][1] = userSignal[i][1];
            }

            isOn = 0;

            pinMode(ir_pin, OUTPUT);
        }

        ~DeviceIR(){}

        void TurnOn()
        {
            if (isOn) return;
            isOn = 1;

            for (int i = 0; i < 2; i++)
            {
                digitalWrite(ir_pin, LOW);
                delay(signal[i][0]);
                digitalWrite(ir_pin, HIGH);
                delay(signal[i][1]);
            }

            digitalWrite(ir_pin, LOW);
        }

        void TurnOff()
        {
            if (!isOn) return;
            isOn = 0;

            for (int i = 0; i < 2; i++)
            {
                digitalWrite(ir_pin, LOW);
                delay(signal[i][0]);
                digitalWrite(ir_pin, HIGH);
                delay(signal[i][1]);
            }

            digitalWrite(ir_pin, LOW);
        }
};

#endif
// Src: https://www.instructables.com/How-to-control-your-TV-with-an-Arduino/