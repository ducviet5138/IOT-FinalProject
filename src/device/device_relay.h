#ifndef DEVICE_RELAY_H
#define DEVICE_RELAY_H

#include "my_map.h"

using namespace std;

class DeviceRelay
{
    private:
        const int relay_light_pin = 13;
        const int relay_room_pin = 32;
        MyMap<int> m_map;

    public:
        DeviceRelay()
        {
            m_map.Add("light", relay_light_pin);
            m_map.Add("room", relay_room_pin);

            pinMode(relay_light_pin, OUTPUT);
            pinMode(relay_room_pin, OUTPUT);
        }

        ~DeviceRelay(){}

        void On(char* param)
        {
            int pin = m_map.Get(param);
            digitalWrite(pin, HIGH);
        }

        void Off(char* param)
        {
            int pin = m_map.Get(param);
            digitalWrite(pin, LOW);
        }        
};

#endif