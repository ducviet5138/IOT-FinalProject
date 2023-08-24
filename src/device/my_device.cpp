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
MyDevice::MyDevice()
{
    ac = new DeviceIR({{1, 203}, {200, 112}});
    fan = new DeviceIR({{1, 112}, {200, 141}});
    tv = new DeviceIR({{1, 141}, {200, 203}});  

    dMode = &deviceMode;
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pw, 6);

    client = new PubSubClient(MQTTServer, 1883, callback, espClient);

    DoOnceSafetyMode = DoOnceWorkingMode = 0;
    SendWarningMessage = 0;

    lastTimeInRoom = 2e+9;
    
    lastReconnectWifiTime = 0;
    lastReconnectMQTTTime = 0;
    lastReconnectCloudTime = 0;

    isReconnectWifi = 1;
    isReconnectMQTT = 1;
    isReconnectCloud = 1;
}

MyDevice::~MyDevice()
{
    delete ac;
    delete fan;
    delete tv;

    delete client;
}

const char* MyDevice::GetChannel(String param)
{
    return (main_channel + param).c_str();
}

void MyDevice::UpdateReconnectStatus()
{
    if (long(millis()) - lastReconnectWifiTime > 300000) isReconnectWifi = 1;
    if (long(millis()) - lastReconnectMQTTTime > 300000) isReconnectMQTT = 1;
    if (long(millis()) - lastReconnectCloudTime > 300000) isReconnectCloud = 1;
}

void MyDevice::ReconnectWifi()
{
    if (WiFi.status() == WL_CONNECTED || !isReconnectWifi) return;

    long startTime = millis();

    WiFi.begin(ssid, pw, 6);
    Serial.print("Connecting to WiFi... ");
    while (WiFi.status() != WL_CONNECTED)
    {
        if (long(millis()) - startTime > 300000)    // 5 mins
        {
            Serial.println("Timeout Wifi!");
            isReconnectWifi = 0;
            lastReconnectWifiTime = millis();
            return;
        }

        delay(1000);
    }
    Serial.println("Connected!");
}

void MyDevice::ReconnectMQTT()
{
    if (!isReconnectWifi || !isReconnectMQTT) return;
    long startTime = millis();

    String clientId = "ESP32Client-" + String(random(0xffff), HEX);

    while (!client->connected())
    {   
        if (long(millis()) - startTime > 300000)
        {
            Serial.println("Timeout MQTT!");
            lastReconnectMQTTTime = millis();
            isReconnectMQTT = 0;
            return;
        }

        Serial.print("Connecting to MQTT server... ");
        if (client->connect(clientId.c_str()))
        {
            Serial.println("Connected!");
            client->subscribe(GetChannel("setmode"));
        }
        else
        {
            Serial.println("Failed!");
            delay(5000);
        }
    }
}

void MyDevice::Reconnect()
{
    UpdateReconnectStatus();
    ReconnectWifi();
    ReconnectMQTT();

    client->loop();
}

void MyDevice::Sync(String param, String value)
{
   client->publish(GetChannel(param), value.c_str());
}

void MyDevice::SendRequestCloud(String param)
{
    if (!isReconnectWifi || !isReconnectCloud) return;

    WiFiClient HttpClient;
    long startTime = millis();

    while (!HttpClient.connect(CloudServer, 80))
    {
        if (long(millis()) - startTime > 300000)
        {
            Serial.println("Timeout Cloud!");
            lastReconnectCloudTime = millis();
            isReconnectCloud = 0;
            return;
        }

        Serial.print("Connecting to cloud... ");
        delay(1000);

        if (HttpClient.connected()) Serial.println("Connected!");
    }

    HttpClient.print(String("GET /update?api_key=XLGHYLZHYRSMC4JJ") + param + " HTTP/1.1\r\n" +
                    "Host: api.thingspeak.com \r\n" +
                    "Connection: close\r\n\r\n");
}

void MyDevice::SyncToServer()
{
    if (!isReconnectWifi) return;

    Sync("temperature", dht.GetTemperature());
    Sync("humid", dht.GetHumid());
    SendRequestCloud("&field1=" + String(dMode->GetWorkingMode())+ "&field2=" + dht.GetTemperature() + "&field3=" + dht.GetHumid());
}

// ========== [Call another device's function] ========== //
// DHT
void MyDevice::updateDHT()
{
    dht.Update();
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
        lastTimeInRoom = millis();
    } else 
    {
        lcdOff();
        if (long(millis()) - lastTimeInRoom > 2000) // 2 mins but set to 2 secs for testing
        {
           relayOff((char*) "light");
           //if tv, fan are turned on, the ir will shine
           tv->TurnOff();
           fan->TurnOff();
        }


        if (long(millis()) - lastTimeInRoom > 4000) // 30 mins but set to 4 secs for testing
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