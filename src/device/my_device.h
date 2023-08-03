#ifndef MY_DEVICE_H
#define MY_DEVICE_H

#include "device/device_dht.h"
#include "device/device_lcd.h"
#include "device/device_pir.h"
#include "device/device_relay.h"
#include "device/device_IR.h"
#include "device/device_mode.h"
#include <PubSubClient.h>
#include <WiFi.h>

class MyDevice
{
    private:
        DeviceDHT dht;
        DeviceLCD lcd;
        DevicePIR pir;
        DeviceRelay relay;
        DeviceIR ir;

        DeviceMode* _dMode;

        const char* server = "broker.hivemq.com";
        const String main_channel = "/GDrpD2J3jxvzQEy7vGOn/";
        WiFiClient espClient;
        PubSubClient client;
        
        const String ssid = "Wokwi-GUEST";
        const String pw = "";
    public:
        void SetUp();

        const char* GetChannel(String pram);
        void reconnect();
        void ReconnectToServer();

        void Sync(String param, String value);
        void SyncTempAndHumid();

        void UpdatePersonStatus();
        void UpdateWorkingMode(bool val);

        void UseIR(char* device)
        {
            ir.UseIR(device);
        }

        void lcdOn()
        {
            lcd.On();
        }

        void lcdOff()
        {
            lcd.Off();
        }

        void lcdPrint()
        {
            dht.Update();
            lcd.Print(dht.GetTemperature(), dht.GetHumid());
        }
};


#endif