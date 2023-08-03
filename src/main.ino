// ============ [Header File] ============
#include <DHTesp.h> // Using DHT22 with ESP32

#include <LiquidCrystal_I2C.h>
#include "device/device_relay.h"
// =======================================






// =========== [Default Value] ===========
// 0. Server

// 1. Working Mode [0: Outside, 1: At Home]
    bool working_mode = 1;

// 2. Person in room [0: Not Availavle, 1: Available]
    bool person_in_room = 0;

// 3. PIR Motion Sensor
    const int pir_pin = 23;

// 4. ESP 32
    String ssid = "Wokwi-GUEST";
    String pw = "";
    


// 6. IR Transmitter (Replaced by LED)
    // Simulate the way IR Transmitter works
    // Src: https://www.instructables.com/How-to-control-your-TV-with-an-Arduino/
    const int ir_pin = 15;
    const int tv_signal[2][2] = {{1, 203}, {200, 112}};
    const int fan_signal[2][2] = {{1, 112}, {200, 141}};
    const int ac_signal[2][2] = {{1, 141}, {200, 203}};

    void UseIR(const int signal[2][2])
    {
        for (int i = 0; i < 2; i++)
        {
            digitalWrite(ir_pin, LOW);
            delay(signal[i][0]);
            digitalWrite(ir_pin, HIGH);
            delay(signal[i][1]);
        }

        digitalWrite(ir_pin, LOW);
    }

// 7. Relay
    

// 8. LCD

// 9. isSendMessage

// 10. Time counter
// =======================================





// ===== [Server - Client Function] ======
/*
void reconnect()
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

*/
// =======================================





// ============ [Setup ESP32] ============
DeviceRelay device_relay;
void setup() 
{
    Serial.begin(115200);

    /*
    // Mode
    working_mode = 1;
    person_in_room = 1;

    // Wifi Connection
    WiFi.begin(ssid, pw);

    // Time Counter

    // Pin
    pinMode(dht_pin, INPUT);
    dht.setup(dht_pin, DHTesp::DHT22);
    pinMode(ir_pin, OUTPUT);
    pinMode(relay_light_pin, OUTPUT);
    pinMode(relay_ac_pin, OUTPUT);
    pinMode(pir_pin, INPUT);

    // LCD
    lcd.init();
    lcd.backlight();

    // Server

    // Temperature and Humid
    temperature = humid = "";

    // Message Status
    */
    device_relay.SetUp();
}
// =======================================






// ============= [Function] ==============
void GetPersonStatus()
{
    person_in_room = digitalRead(pir_pin);
}

/*
void ReadTempAndHumid()
{
    TempAndHumidity data = dht.getTempAndHumidity();
    temperature = String(data.temperature, 2);
    humid = String(data.humidity, 1);
}

void SyncTempAndHumid()
{
    client.publish(GetChannel("temperature"), temperature.c_str());
    client.publish(GetChannel("humid"), humid.c_str());
}
*/

void HandleWorkingMode()
{
    /*
    isSendMessage_off = 0;

    if (!isSendMessage_on)
    {
        isSendMessage_on = 1;
        String Warning_Mess = "0";
        client.publish("/GDrpD2J3jxvzQEy7vGOn/warning", Warning_Mess.c_str());
    }

    if (person_in_room)
    {
        Serial.println("In");
        PowerOn();
        if (temperature.toFloat() > 30)
            UseIR();

        lcd.setCursor(0, 0);
        lcd.print("Temp: " + temperature + " " + String(char(223)) + "C ");
        lcd.setCursor(0, 1);
        lcd.print("Humid: " + humid + " %");
        s_time_counter = millis();
    }
    else
    {
        Serial.println("Out");
        lcd.clear();

        if (millis() - s_time_counter >= 6000)
        {
            PowerOff();
            // Turn Off Light
        }

        if (millis() - s_time_counter >= 1800000) // 30 mins
        {
            UseIR();
            // Turn Off AC
        }
    }
    */
}

void HandleSafetyMode()
{
    /*
    if (person_in_room && !isSendMessage_off)
    {
        String Warning_Mess = "1";
        client.publish("/GDrpD2J3jxvzQEy7vGOn/warning", Warning_Mess.c_str());
        isSendMessage_off = 1;
    }

    isSendMessage_on = 0;
    */
}
// =======================================





// ============ [Loop ESP32] =============
void loop() {
    /*
    if (!client.connected()) reconnect();
    client.loop();
  
    ReadTempAndHumid();
    SyncTempAndHumid();
    GetPersonStatus();
  
    if (working_mode) HandleWorkingMode(); else HandleSafetyMode();
    */
    char temp[] = "light";
    device_relay.On(temp);
    delay(1000);
    device_relay.Off(temp);
    delay(1000);
}
// =======================================
