#include "device/my_device.h"

MyDevice md;

void setup()
{
    Serial.begin(115200);
    md.SetUp();
}

void loop()
{
    // md.ReconnectToServer();

    md.UseIR("tv");
    delay(1000);
    md.lcdOn();
    md.lcdPrint();
    delay(1000);
    md.lcdOff();
}
