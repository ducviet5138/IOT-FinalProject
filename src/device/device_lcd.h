#ifndef DEVICE_LCD_H
#define DEVICE_LCD_H

#include <LiquidCrystal_I2C.h>

class DeviceLCD
{
    private:
        LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

    public:
        void SetUp()
        {
            lcd.init();
            lcd.backlight();
        }

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
            lcd.print("Humid: " + humid + " %");
        }
};

#endif