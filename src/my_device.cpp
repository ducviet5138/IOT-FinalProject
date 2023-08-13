#include "my_device.h"
#include "device_mode.h"

DeviceMode deviceMode;
void callback(char* topic, byte* message, unsigned int length)
{
    String buffer = "";
    for (int i = 0; i < length; i++) buffer += (char)message[i];
    Serial.println(buffer);

    if (buffer == "WorkingMode") deviceMode.UpdateMode(1);
    if (buffer == "SafetyMode") deviceMode.UpdateMode(0);
}



// ========== [Define methods for MyDevice] ========== //
void MyDevice::SetUp()
{
    dht.SetUp();
    lcd.SetUp();
    pir.SetUp();
    relay.SetUp();

    ac = new DeviceIR({{1, 203}, {200, 112}});
    fan = new DeviceIR({{1, 112}, {200, 141}});
    tv = new DeviceIR({{1, 141}, {200, 203}});  

    ac->SetUp();
    fan->SetUp();
    tv->SetUp();

    dMode = &deviceMode;
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pw, 6);

    client = PubSubClient(server, 1883, callback, espClient);

    DoOnceSafetyMode = DoOnceWorkingMode = 0;
    SendWarningMessage = 0;

    countTime = 2e+9;
}

MyDevice::~MyDevice()
{
    delete ac;
    delete fan;
    delete tv;
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

void MyDevice::SendRequest(String param)
{
    httpService.SendRequest(param);
}



// ========== [Call another device's function] ========== //
// DHT
void MyDevice::SyncTempAndHumid()
{
    dht.Update();
    Sync("temperature", dht.GetTemperature());
    Sync("humid", dht.GetHumid());
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
    if (dMode->GetWorkingMode()) HandleWorkingMode();
    else HandleSafetyMode();
}



// Working Mode
void MyDevice::HandleWorkingMode()
{
    if (!DoOnceWorkingMode)
    {
        DoOnceSafetyMode = SendWarningMessage = 0;

        // Sync Current Mode
        Sync("currentmode", "1");
        
        // Turn on room's electricity
        relayOn((char*) "room");

        DoOnceWorkingMode = 1;
    }

    if (pir.GetPersonStatus())
    {
        relayOn((char*) "light");
        lcdOn();
        lcdPrint();

        if (dht.GetTemperature().toFloat() > 30) ac->TurnOn();
        countTime = millis();
    } else 
    {
        lcdOff();
        if (long(millis()) - countTime > 2000)
        {
           relayOff((char*) "light");
           //if tv, fan are turned on, the ir will shine
           tv->TurnOff();
           fan->TurnOff();
        }


        if (long(millis()) - countTime > 4000)
        {
            //ac are turned on, the ir will shine
            ac->TurnOff();
        }
         
    }
}



// Safety Mode
void MyDevice::HandleSafetyMode()
{   
    if (pir.GetPersonStatus() && !SendWarningMessage)
    {
        Sync("warning", "1");
        SendWarningMessage = 1;
    }
    
    if (!DoOnceSafetyMode)
    {
        DoOnceWorkingMode = 0;

        // Sync Current Mode
        Sync("currentmode", "0");

        // Turn off electric devices
        tv->TurnOff();
        fan->TurnOff();
        ac->TurnOff();

        // Turn off light and room's electricity
        relayOff((char*) "light");
        relayOff((char*) "room");

        // Turn off LCD
        lcdOff();

        DoOnceSafetyMode = 1;
    }
}