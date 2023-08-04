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

        const char* server = "broker.emqx.io";
        const String main_channel = "/GDrpD2J3jxvzQEy7vGOn/";
        WiFiClient espClient;
        PubSubClient client;
        
        const String ssid = "Wokwi-GUEST";
        const String pw = "";

        bool SendMessage_WorkingMode;
        bool SendMessage_SafetyMode;

        bool TurnOffDevices_SafetyMode;
    public:
        void SetUp();

        const char* GetChannel(String pram);
        void reconnect();
        void ReconnectToServer();
        void Sync(String param, String value);
        void UpdateWorkingMode(bool val);

        // DHT
        void SyncTempAndHumid();
        void UpdateDHT();

        // IR
        void UseIR(char* device);

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