#include "my_device.h"
#include "device/device_mode.h"


// ========== [Global variables] ========== //
bool isPrint_SafetyMode = 0;
bool isPrint_WorkingMode = 0;

DeviceMode dMode;
void callback(char* topic, byte* message, unsigned int length)
{
    String buffer = "";
    for (int i = 0; i < length; i++) buffer += (char)message[i];
    Serial.println(buffer);

    if (buffer == "WorkingMode") dMode.UpdateWorkingMode(1);
    if (buffer == "SafetyMode") dMode.UpdateWorkingMode(0);
}

void MyDevice::SetUp()
{
    dht.SetUp();
    lcd.SetUp();
    pir.SetUp();
    relay.SetUp();
    ir.SetUp();

    _dMode = &dMode;

    client = PubSubClient(espClient);
    client.setServer(server, 1883);
    client.setCallback(callback);

    WiFi.begin(ssid, pw);

    SendMessage_SafetyMode = SendMessage_WorkingMode = 0;

    TurnOffDevices_SafetyMode = 0;
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

void MyDevice::ReconnectToServer()
{
    reconnect();
    client.loop();
}

void MyDevice::Sync(String param, String value)
{
    Serial.println("Is pub: " + String(client.publish(GetChannel(param), value.c_str())));
}

const char* MyDevice::GetChannel(String param)
{
    return (main_channel + param).c_str();
}

void MyDevice::UpdateWorkingMode(bool val)
{
    _dMode->UpdateWorkingMode(val);
}

// ========== [Call another device's function] ========== //
// DHT
void MyDevice::SyncTempAndHumid()
{
    Sync("temperature", dht.GetTemperature());
    Sync("humid", dht.GetHumid());
}

void MyDevice::UpdateDHT()
{
    dht.Update();
}



// IR
void MyDevice::UseIR(char* device)
{
    ir.UseIR(device);
}



// LCD
void MyDevice::lcdOn()
{
    lcd.On();
}

void MyDevice::lcdOff()
{
    lcd.Off();
}

void MyDevice::lcdPrint()
{
    dht.Update();
    lcd.Print(dht.GetTemperature(), dht.GetHumid());
}



// Relay
void MyDevice::relayOn(char* param)
{
    relay.On(param);
}

void MyDevice::relayOff(char* param)
{
    relay.Off(param);
}

// =============== [Handle Mode] =============== //
// Choose suitable mode
void MyDevice::ChooseSuitableMode()
{
    if (_dMode->GetWorkingMode()) HandleWorkingMode();
    else HandleSafetyMode();
}



// Working Mode
void MyDevice::HandleWorkingMode()
{
    SendMessage_SafetyMode = TurnOffDevices_SafetyMode = 0;
    if (!isPrint_WorkingMode)
    {
        Serial.println("Working Mode");
        isPrint_WorkingMode = 1;
        isPrint_SafetyMode = 0;
    }
}



// Safety Mode
void MyDevice::HandleSafetyMode()
{
    if (!isPrint_SafetyMode)
    {
        Serial.println("Safety Mode");
        isPrint_SafetyMode = 1;
        isPrint_WorkingMode = 0;
    }

    SendMessage_WorkingMode = 0;
    
    if (pir.GetPersonStatus() && !SendMessage_SafetyMode)
    {
        String message = "1";
        Sync("warning", message.c_str());
        SendMessage_SafetyMode = 1;
    }
    
    if (!TurnOffDevices_SafetyMode)
    {
        String tv = "tv";
        String fan = "fan";
        String air_conditioner = "ac";

        // Turn off electric devices
        UseIR((char*) tv.c_str());
        UseIR((char*) fan.c_str());
        UseIR((char*) air_conditioner.c_str());
        
        String light = "light";
        String room = "room";

        // Turn off light and room's electricity
        relayOff((char*) light.c_str());
        relayOff((char*) room.c_str());

        // Turn off LCD
        lcdOff();

        TurnOffDevices_SafetyMode = 1;
    }
}