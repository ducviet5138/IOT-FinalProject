# 1 "C:\\Users\\DUCVIE~1\\AppData\\Local\\Temp\\tmp5j56ikub"
#include <Arduino.h>
# 1 "C:/Users/ducviet5138/Desktop/Wokwi/src/main.ino"
#include "device/my_device.h"

MyDevice md;
void setup();
void loop();
#line 5 "C:/Users/ducviet5138/Desktop/Wokwi/src/main.ino"
void setup()
{
    Serial.begin(115200);
    md.SetUp();
}

void loop()
{
    md.ReconnectToServer();
    md.SyncTempAndHumid();
    md.ChooseSuitableMode();
}