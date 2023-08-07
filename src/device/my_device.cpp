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



// ========== [Define methods for MyDevice] ========== //
void MyDevice::SetUp()
{
    dht.SetUp();
    lcd.SetUp();
    pir.SetUp();
    relay.SetUp();
    ir.SetUp();

    _dMode = &dMode;
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pw, 6);

    client = PubSubClient(server, 1883, callback, espClient);

    DoOnceSafetyMode = DoOnceWorkingMode = 0;
    SendWarningMessage = 0;

    countTime = 0;

    UseAC = 0;
}

const char* MyDevice::GetChannel(String param)
{
    return (main_channel + param).c_str();
}

void MyDevice::reconnect()
{
    while (!client.connected())
    {
        String clientId = "ESP32Client-" + String(random(0xffff), HEX);

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
   client.publish(GetChannel(param), value.c_str());
}

void MyDevice::UpdateWorkingMode(bool val)
{
    _dMode->UpdateWorkingMode(val);
}



// ========== [Call another device's function] ========== //
// DHT
void MyDevice::SyncTempAndHumid()
{
    dht.Update();
    Sync("temperature", dht.GetTemperature());
    Sync("humid", dht.GetHumid());
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
    if (!isPrint_WorkingMode)
    {
        Serial.println("Working Mode");
        isPrint_WorkingMode = 1;
        isPrint_SafetyMode = 0;
    }

    if (!DoOnceWorkingMode)
    {
        DoOnceSafetyMode = SendWarningMessage = 0;
        // Remember to delete
        isPrint_SafetyMode = 0;

        // Turn on room's electricity
        relayOn((char*) "room");

        if (!UseAC and dht.GetTemperature().toFloat() > 30)
        {
            UseIR((char*) "ac");
            UseAC = 1;
        }

        DoOnceWorkingMode = 1;
    }

    if (pir.GetPersonStatus())
    {
        relayOn((char*) "light");
        lcdOn();
        lcdPrint();

        countTime = millis();
    } else 
    {
        lcdOff();

        if (millis() - countTime > 20000)
        {
            relayOff((char*) "light");
            UseIR((char*) "tv");
            UseIR((char*) "fan");
        }

        if (millis() - countTime > 30000 and UseAC)
        {
            UseIR((char*) "ac");
            UseAC = 0;
        }
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
    
    if (pir.GetPersonStatus() && !SendWarningMessage)
    {
        String message = "1";
        Sync("warning", message.c_str());
        SendWarningMessage = 1;
    }
    
    if (!DoOnceSafetyMode)
    {
        DoOnceWorkingMode = 0;
        // Remember to delete
        isPrint_WorkingMode = 0;

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

        DoOnceSafetyMode = 1;
    }
}