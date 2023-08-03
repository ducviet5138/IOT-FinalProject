#ifndef DEVICE_DHT_H
#define DEVICE_DHT_H

#include <DHTesp.h>

class DeviceDHT
{
    private:
        DHTesp dht;
        const int dht_pin = 15;
        String temperature;
        String humid;

    public:
        void SetUp()
        {
            dht.setup(dht_pin, DHTesp::DHT22);
            temperature = humid = "";
        }

        void Update()
        {
            TempAndHumidity data = dht.getTempAndHumidity();
            temperature = String(data.temperature, 2);
            humid = String(data.humidity, 1);
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