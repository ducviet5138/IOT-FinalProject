#ifndef DEVICE_RELAY_H
#define DEVICE_RELAY_H

#include <Arduino.h>
#include <map>

using namespace std;

class DeviceRelay
{
    private:
        const int relay_light_pin = 13;
        const int relay_room_pin = 32;
        map<String, int> MyMap;

    public:
        DeviceRelay()
        {
            MyMap["light"] = relay_light_pin;
            MyMap["room"] = relay_room_pin;
        }

        void On(String param)
        {
            int pin = MyMap[param];
            digitalWrite(pin, HIGH);
        }

        void Off(String param)
        {
            int pin = MyMap[param];
            digitalWrite(pin, LOW);
        }
};

#endif