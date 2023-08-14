#ifndef MY_DEVICE_H
#define MY_DEVICE_H

#include "device_dht.h"
#include "device_lcd.h"
#include "device_pir.h"
#include "device_relay.h"
#include "device_IR.h"
#include "device_mode.h"
#include <PubSubClient.h>
#include <WiFi.h>

class MyDevice
{
    private:
        DeviceDHT dht;
        DeviceLCD lcd;
        DevicePIR pir;
        DeviceRelay relay;
        DeviceIR *ac;
        DeviceIR *tv;
        DeviceIR *fan;

        DeviceMode* dMode;

        const char* server = "broker.emqx.io";
        const String main_channel = "/GDrpD2J3jxvzQEy7vGOn/";
        WiFiClient espClient;
        PubSubClient client;
        
        const String ssid = "Wokwi-GUEST";
        const String pw = "";

        bool DoOnceSafetyMode;
        bool DoOnceWorkingMode;
        bool SendWarningMessage;

        long countTime;
    public:
        void SetUp();
        ~MyDevice();
        
        const char* GetChannel(String param);
        void reconnect();
        void ReconnectToServer();
        void Sync(String param, String value);

        void SendRequest(String param);

        // DHT
        void SyncTempAndHumid();

        // LCD
        void lcdOn();
        void lcdOff();
        void lcdPrint();

        // Relay
        void relayOn(char* param);
        void relayOff(char* param);

        // Mode [0: Safety Mode, 1: Working Mode]
        void ChooseSuitableMode();
        void HandleSafetyMode();
        void HandleWorkingMode();
};


#endif