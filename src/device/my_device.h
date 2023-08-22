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

        const char* MQTTServer = "broker.emqx.io";
        const char* CloudServer = "api.thingspeak.com";
        const String main_channel = "/GDrpD2J3jxvzQEy7vGOn/";
        WiFiClient espClient;
        PubSubClient *client;
        
        const String ssid = "Wokwi-GUEST";
        const String pw = "";

        bool DoOnceSafetyMode;
        bool DoOnceWorkingMode;
        bool SendWarningMessage;

        long lastTimeInRoom;
        
        long lastReconnectWifiTime;
        long lastReconnectMQTTTime;
        long lastReconnectCloudTime;

        bool isReconnectWifi;
        bool isReconnectMQTT;
        bool isReconnectCloud;
    public:
        MyDevice();
        ~MyDevice();
        
        const char* GetChannel(String param);
        void ReconnectWifi();
        void ReconnectMQTT();
        void Reconnect();
        void UpdateReconnectStatus();
        void Sync(String param, String value);

        void SendRequestCloud(String param);
        void SyncToServer();

        // DHT
        void updateDHT();
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