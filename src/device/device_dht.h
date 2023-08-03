#ifndef DEVICE_DHT_H
#define DEVICE_DHT_H

#include <DHTesp.h>

class DeviceDHT
{
    private:
        DHTesp dht;
        const int dht_pin = 4;
        String temperature;
        String humid;

    public:
        void SetUp()
        {
            pinMode(dht_pin, INPUT);
            dht.setup(dht_pin, DHTesp::DHT22);

            temperature = humid = "";
        }

        void Update()
        {
            temperature = String(dht.getTemperature());
            humid = String(dht.getHumidity());
        }

        String GetTemperature()
        {
            return temperature;
        }

        String GetHumid()
        {
            return humid;
        }
};

#endif