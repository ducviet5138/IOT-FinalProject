#ifndef DEVICE_IR_H
#define DEVICE_IR_H
#include "my_map.h"
#include <vector>

using namespace std;

class DeviceIR
{
    private:
        const int ir_pin = 4;
        MyMap<vector<vector<int>>> _devices;
        
    public:
        DeviceIR(){
            _devices.Add("tv", {{1, 203}, {200, 112}});
            _devices.Add("fan", {{1, 112}, {200, 141}});
            _devices.Add("ac", {{1, 141}, {200, 203}});
        }

        void SetUp(){
            pinMode(ir_pin, OUTPUT);
        }

        void UseIR(char* device_name){
            for (int i = 0; i < 2; i++)
            {
                digitalWrite(ir_pin, LOW);
                delay(_devices.Get(device_name)[i][0]);
                digitalWrite(ir_pin, HIGH);
                delay(_devices.Get(device_name)[i][1]);
            }

            digitalWrite(ir_pin, LOW);
        }
};


#endif