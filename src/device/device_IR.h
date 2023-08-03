#ifndef DEVICE_IR_H
#define DEVICE_IR_H
#include <map>
#include <arduino.h>

using namespace std;

class DeviceIR
{
    private:
        const int ir_pin = 15;
        map<char*, const int [2][2]> _devices;
        
    public:
        DeviceIR();
        void UseIR(const int [2][2]);
};


DeviceIR::DeviceIR()
{
    pinMode(ir_pin, OUTPUT);
    
    _devices["tv"] = {{1, 203}, {200, 112}};
    _devices["fan"] = {{1, 112}, {200, 141}};
    _devices["ac"] = {{1, 141}, {200, 203}};
}


DeviceIR::UseIR(char* device_name)
{
    for (int i = 0; i < 2; i++)
    {
        digitalWrite(ir_pin, LOW);
        delay(_devicesp[device_name][i][0]);
        digitalWrite(ir_pin, HIGH);
        delay(_devices[device_name][i][1]);
    }

    digitalWrite(ir_pin, LOW);
}
#endif