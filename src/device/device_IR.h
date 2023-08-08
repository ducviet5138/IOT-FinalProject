#ifndef DEVICE_IR_H
#define DEVICE_IR_H
#include "my_map.h"
#include <vector>

using namespace std;

class DeviceIR
{
    private:
        const int ir_pin = 4;
        vector<vector<int>> signal;
        bool isOn;
        
    public:
        DeviceIR(vector<vector<int>> userSignal)
        {
            swap(signal, userSignal);
            isOn = 0;
        }

        void SetUp()
        {
            pinMode(ir_pin, OUTPUT);
        }

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