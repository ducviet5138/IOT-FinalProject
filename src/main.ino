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

// Text in wokwi: https://wokwi.com/projects/341085054102930004

// Telegram section
//      Telegram token: 6370226403:AAFalsUvcL8HUX592rCplIo4qq4AhxEYo-w
//      Chat id (Việt): 955526463
//      Chat id (Quân): 5011947142