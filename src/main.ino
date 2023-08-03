// ============ [Header File] ============
#include <DHTesp.h> // Using DHT22 with ESP32

#include <LiquidCrystal_I2C.h>
#include "device/device_relay.h"
#include "device/device_IR.h"
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
    // const int ir_pin = 15;
    // const int tv_signal[2][2] = {{1, 203}, {200, 112}};
    // const int fan_signal[2][2] = {{1, 112}, {200, 141}};
    // const int ac_signal[2][2] = {{1, 141}, {200, 203}};

    // void UseIR(const int signal[2][2])
    // {
    //     for (int i = 0; i < 2; i++)
    //     {
    //         digitalWrite(ir_pin, LOW);
    //         delay(signal[i][0]);
    //         digitalWrite(ir_pin, HIGH);
    //         delay(signal[i][1]);
    //     }

    //     digitalWrite(ir_pin, LOW);
    // }

// 7. Relay
    

// 8. LCD

// 9. isSendMessage

// 10. Time counter
// =======================================





// ===== [Server - Client Function] ======
/*


*/
// =======================================





// ============ [Setup ESP32] ============
DeviceRelay device_relay;
DeviceIR device_ir;
void setup() 
{
    Serial.begin(115200);

    /*
    // Mode
    

    // Wifi Connection
    WiFi.begin(ssid, pw);

    // Time Counter



    // Server


    // Message Status
    */
    device_relay.SetUp();
    device_ir.SetUp();
}
// =======================================






// ============= [Function] ==============


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
    char temp1[] = "tv";
    device_relay.On(temp);
    delay(1000);
    device_relay.Off(temp);
    delay(1000);

    device_ir.UseIR(temp1);
}
// =======================================
