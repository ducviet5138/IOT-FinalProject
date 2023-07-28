// ===============================
// Header File Here
#include <DHTesp.h> // Using DHT22 with ESP32
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <PubSubClient.h>



// ===============================
// Default Value Here
  // 0. Server
  const char* server = "broker.hivemq.com";

  // 1. Working Mode [0: Outside, 1: At Home]
  bool working_mode = 0;

  // 2. Person in room [0: Not Availavle, 1: Available]
  bool person_in_room = 0;

  // 3. PIR
  const int PIR_pin = 13;

  // 4. ESP 32
  String ssid = "Wokwi-GUEST";
  String pw = "";
  WiFiClient espClient;
  PubSubClient client(espClient);

  // 5. DHT 22
  DHTesp dht;
  const int DHT_pin = 12;
  String temperature = "";
  String humid = "";

  // 6. IR Transmitter (Replaced by LED)
  const int IR_pin = 15;
  void UseIR()
  {
    digitalWrite(IR_pin, HIGH);
    delay(1000);
    digitalWrite(IR_pin, LOW);
    delay(1000);
  }

  // 7. Relay
  const int Relay_pin = 23;
  void PowerOn()
  {
    digitalWrite(Relay_pin, HIGH);
  }

  void PowerOff()
  {
    digitalWrite(Relay_pin, LOW);
  }

  // 8. LCD
  LiquidCrystal_I2C lcd(0x27, 16, 2);

  // 9. isSendMessage
  bool isSendMessage_on, isSendMessage_off = 0;

  // 10. Time counter
  long s_time_counter = 0;
  long e_time_counter = 0;

// ===============================
// Set function for client
void reconnect() 
{ 
  while (!client.connected()) 
  {
    String clientId = "ESP32Client-21127112-21127141-21127203";

    Serial.print("Connecting to server...");
    if (client.connect(clientId.c_str())) 
    {
      Serial.println(" Connected!"); 
      client.subscribe("/GDrpD2J3jxvzQEy7vGOn/text");
    } else 
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



// ===============================
// Setup Arduino Here
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Mode
  working_mode = 1;
  person_in_room = 1;

  // Wifi Connection
  WiFi.begin(ssid, pw);

  // Time 
  s_time_counter = e_time_counter = 0;

  // Pin
  pinMode(PIR_pin, INPUT);
  dht.setup(DHT_pin, DHTesp::DHT22);
  pinMode(IR_pin, OUTPUT);
  pinMode(Relay_pin, OUTPUT);

  // LCD
  Wire.begin(4, 2);
  lcd.init();
  lcd.backlight();

  // Server
  client.setServer(server, 1883);
  client.setCallback(callback);
  
  // Temperature and Humid
  temperature = humid = "";
}



// ===============================
// Function Here
void GetPersonStatus()
{
  person_in_room = digitalRead(PIR_pin);
}

void ReadTempAndHumid()
{
  TempAndHumidity data = dht.getTempAndHumidity();
  temperature = String(data.temperature, 2);
  humid = String(data.humidity, 1);
}

void SyncTempAndHumid()
{
  client.publish("/GDrpD2J3jxvzQEy7vGOn/temp", temperature.c_str());
  client.publish("/GDrpD2J3jxvzQEy7vGOn/humid", humid.c_str());
}

void HandleWorkingMode()
{
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
    if (temperature.toFloat() > 30) UseIR();

    lcd.setCursor(0, 0);
    lcd.print("Temp: " + temperature + " " + String(char(223)) + "C ");
    lcd.setCursor(0, 1);
    lcd.print("Humid: " + humid + " %");
    s_time_counter = millis();
  } else 
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
}


void HandleSafetyMode()
{
   if(person_in_room && !isSendMessage_off)
      {
        String Warning_Mess = "1";
        client.publish("/GDrpD2J3jxvzQEy7vGOn/warning", Warning_Mess.c_str());
        isSendMessage_off = 1;
      }

      isSendMessage_on = 0;
}



// ===============================
// Inf Loop Here
void loop() {
  if (!client.connected()) reconnect();
  client.loop();
  
  ReadTempAndHumid();
  SyncTempAndHumid();
  GetPersonStatus();
  
  if (working_mode) HandleWorkingMode(); else HandleSafetyMode();
}
