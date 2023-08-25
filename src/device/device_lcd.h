#ifndef DEVICE_LCD_H
#define DEVICE_LCD_H

#include <LiquidCrystal_I2C.h>

class DeviceLCD
{
    private:
        LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

    public:
        DeviceLCD()
        {
            lcd.init();
            lcd.backlight();
        }

        ~DeviceLCD(){}

        void On()
        {
            lcd.backlight();
            lcd.display();
        }

        void Off()
        {
            lcd.noBacklight();
            lcd.noDisplay();
        }

        void Print(String temperature, String humid)
        {
            lcd.setCursor(0, 0);
            lcd.print("Temp: " + temperature + " " + String(char(223)) + "C ");
            lcd.setCursor(0, 1);
            lcd.print("Humid: " + humid + " % ");
        }
};

#endif
// The way to turn on/off LCD
// Src: https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library/blob/master/LiquidCrystal_I2C.h

// Special character
// Src: https://docs.wokwi.com/parts/wokwi-lcd1602