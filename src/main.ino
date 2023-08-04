#include "device/my_device.h"

MyDevice md;
long countLoop = 0;

void setup()
{
    Serial.begin(115200);
    md.SetUp();
}

void loop()
{
    ++countLoop;
    Serial.println("Loop: "+ String(countLoop));
    md.ReconnectToServer();

    md.UpdateDHT();
    md.SyncTempAndHumid();

    md.ChooseSuitableMode();
}
