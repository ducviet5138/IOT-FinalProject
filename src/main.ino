// ============ [Header File] ============
#include <DHTesp.h> // Using DHT22 with ESP32
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <PubSubClient.h>
// =======================================






// =========== [Default Value] ===========
// 0. Server
    const char* server = "broker.hivemq.com";
    const String main_channel = "/GDrpD2J3jxvzQEy7vGOn/";
    const char* GetChannel(String pram)
    {
        return (main_channel + pram).c_str();
    }

// 1. Working Mode [0: Outside, 1: At Home]
    bool working_mode = 1;

// 2. Person in room [0: Not Availavle, 1: Available]
    bool person_in_room = 0;

// 3. PIR Motion Sensor
    const int pir_pin = 23;

// 4. ESP 32
    String ssid = "Wokwi-GUEST";
    String pw = "";
    WiFiClient espClient;
    PubSubClient client(espClient);

// 5. DHT 22
    DHTesp dht;
    const int dht_pin = 2;
    String temperature = "";
    String humid = "";

// 6. IR Transmitter (Replaced by LED)
    const int ir_pin = 15;

// 7. Relay
    const int relay_light_pin = 13;
    const int relay_ac_pin = 32;

// 8. LCD
    LiquidCrystal_I2C lcd(0x27, 16, 2);
    const int lcd_sda_pin = 34;
    const int lcd_scl_pin = 35;

// 9. isSendMessage
    bool isSendMessage_on, isSendMessage_off = 0;

// 10. Time counter
    long s_time_counter = 0;
    long e_time_counter = 0;
// =======================================





// ===== [Server - Client Function] ======
void reconnect()
{
    while (!client.connected())
    {
       String clientId = "ESP32Client-21127112-21127141-21127203";

        Serial.print("Connecting to server...");
        if (client.connect(clientId.c_str()))
        {
            Serial.println(" Connected!");
            client.subscribe(GetChannel("WorkingMode"));
        }
        else
        {
            Serial.println(" Failed!");
            delay(5000);
        }
    }
}

void callback(char* topic, byte* message, unsigned int length) 
{
    String buffer = "";
    for (int i = 0; i < length; i++) buffer += char(message[i]);

    Serial.println(buffer);
    working_mode = (buffer == "mode_on");
}
// =======================================





// ============ [Setup ESP32] ============
void setup() 
{
    Serial.begin(115200);

    // Mode
    working_mode = 1;
    person_in_room = 1;

    // Wifi Connection
    WiFi.begin(ssid, pw);

    // Time
    s_time_counter = e_time_counter = 0;

    // Pin
    pinMode(dht_pin, INPUT);
    dht.setup(dht_pin, DHTesp::DHT22);
    pinMode(ir_pin, OUTPUT);
    pinMode(ir_pin, OUTPUT);

    // LCD
    Wire.begin(lcd_scl_pin, lcd_sda_pin);
    lcd.init();
    lcd.backlight();

    // Server
    client.setServer(server, 1883);
    client.setCallback(callback);

    // Temperature and Humid
    temperature = humid = "";
}
// =======================================






// ============= [Function] ==============
void GetPersonStatus()
{
    person_in_room = digitalRead(pir_pin);
}

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
    if (!client.connected()) reconnect();
    client.loop();
  
    ReadTempAndHumid();
    SyncTempAndHumid();
    GetPersonStatus();
  
    if (working_mode) HandleWorkingMode(); else HandleSafetyMode();
}
// =======================================
