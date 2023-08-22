#include "device/my_device.h"

MyDevice *md = NULL;

void setup()
{
    Serial.begin(115200);
    md = new MyDevice();
}

void loop()
{
    md->updateDHT();
    md->Reconnect();
    md->SyncToServer();
    md->ChooseSuitableMode();
}
