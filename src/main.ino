#include "device/my_device.h"

MyDevice md;

void setup()
{
    Serial.begin(115200);
    md.SetUp();
}

void loop()
{
    md.ReconnectToServer();

//    md.UpdatePersonStatus();
//    md.relayOn("light");
//    delay(2000);
//    md.relayOff("light");
//    delay(2000);
}
