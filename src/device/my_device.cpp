#include "my_device.h"
#include "device/device_mode.h"

DeviceMode dMode;
void callback(char* topic, byte* message, unsigned int length)
{
    String buffer = "";
    for (int i = 0; i < length; i++) buffer += (char)message[i];
    Serial.println(buffer);

    if (buffer == "WorkingMode") dMode.UpdateWorkingMode(true);
    if (buffer == "SafetyMode") dMode.UpdateWorkingMode(false);
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

void MyDevice::UpdatePersonStatus()
{
    bool newStatus = pir.IsMotion();
    _dMode->UpdatePersonStatus(newStatus);
    if (newStatus)
        Serial.println("Person is in the room");
    else
        Serial.println("Person is not in the room");
}

void MyDevice::UpdateWorkingMode(bool val)
{
    _dMode->UpdateWorkingMode(val);
}

// ========== [Call another device's function] ========== //
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
}



// Safety Mode
void MyDevice::HandleSafetyMode()
{
}