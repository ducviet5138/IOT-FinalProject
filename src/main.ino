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

// Config: https://docs.wokwi.com/vscode/project-config
// Example: https://github.com/wokwi/esp32-ntp-clock


//chat id (Việt): 955526463
//chat id (Quân): 5011947142