#include "my_device.h"

MyDevice md;

void setup()
{
    Serial.begin(115200);
    md.SetUp();
}

void loop()
{
    md.ReconnectToServer();
    md.SyncTempAndHumid();
    md.SyncToCloud();
    md.ChooseSuitableMode();
}
