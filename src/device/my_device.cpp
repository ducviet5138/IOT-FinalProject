#include "my_device.h"

void MyDevice::SetUp()
{
    dht.SetUp();
    lcd.SetUp();
    pir.SetUp();
    relay.SetUp();
    ir.SetUp();

    working_mode = 1;
    person_in_room = 1;

    client = PubSubClient(espClient);
    client.setServer(server, 1883);
    client.setCallback(callback);

    WiFi.begin(ssid, pw);
}

void MyDevice::reconnect()
{
    while (!client.connected())
    {
        String clientId = "ESP32Client-21127112-21127141-21127203";

        Serial.print("Connecting to server... ");
        if (client.connect(clientId.c_str()))
        {
            Serial.println("Connected!");
            client.subscribe(GetChannel("WorkingMode"));
        }
        else
        {
            Serial.println("Failed!");
            delay(5000);
        }
    }
}

static void MyDevice::callback(char* topic, byte* message, unsigned int length)
{
    String buffer = "";
    for (int i = 0; i < length; i++) buffer += char(message[i]);    
    Serial.println(buffer);   
}

void MyDevice::ReconnectToServer()
{
    if (!client.connected()) reconnect();
    client.loop();
}

void MyDevice::Sync(String param, String value)
{
    client.publish(GetChannel(param), value.c_str());
}

void MyDevice::SyncTempAndHumid()
{
    Sync("temperature", dht.GetTemperature());
    Sync("humid", dht.GetHumid());
}

const char* MyDevice::GetChannel(String param)
{
    return (main_channel + param).c_str();
}

void MyDevice::GetPersonStatus()
{
    person_in_room = pir.IsMotion();
}