#ifndef DEVICE_DHT_H
#define DEVICE_DHT_H

#include <DHTesp.h>

class DeviceDHT
{
    private:
        DHTesp dht;
        const int dht_pin = 15;
        TempAndHumidity data;

    public:
        DeviceDHT()
        {
            dht.setup(dht_pin, DHTesp::DHT22);
        }

        ~DeviceDHT(){}

        void Update()
        {
            data = dht.getTempAndHumidity();
        }

        String GetTemperature()
        {
            return String(data.temperature, 2);
        }

        String GetHumid()
        {
            return String(data.humidity, 2);
        }
};

#endif