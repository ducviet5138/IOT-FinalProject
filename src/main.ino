
void HandleWorkingMode()
{
    /*
    isSendMessage_off = 0;

    if (!isSendMessage_on)
    {
        isSendMessage_on = 1;
        String Warning_Mess = "0";
        client.publish("/GDrpD2J3jxvzQEy7vGOn/warning", Warning_Mess.c_str());
    }

    if (person_in_room)
    {
        Serial.println("In");
        PowerOn();
        if (temperature.toFloat() > 30)
            UseIR();

        lcd.setCursor(0, 0);
        lcd.print("Temp: " + temperature + " " + String(char(223)) + "C ");
        lcd.setCursor(0, 1);
        lcd.print("Humid: " + humid + " %");
        s_time_counter = millis();
    }
    else
    {
        Serial.println("Out");
        lcd.clear();

        if (millis() - s_time_counter >= 6000)
        {
            PowerOff();
            // Turn Off Light
        }

        if (millis() - s_time_counter >= 1800000) // 30 mins
        {
            UseIR();
            // Turn Off AC
        }
    }
    */
}

void HandleSafetyMode()
{
    /*
    if (person_in_room && !isSendMessage_off)
    {
        String Warning_Mess = "1";
        client.publish("/GDrpD2J3jxvzQEy7vGOn/warning", Warning_Mess.c_str());
        isSendMessage_off = 1;
    }

    isSendMessage_on = 0;
    */
}
// =======================================


void setup() {

}


// ============ [Loop ESP32] =============
void loop() {
    /*
    
    ReadTempAndHumid();
    SyncTempAndHumid();
    GetPersonStatus();
  
    if (working_mode) HandleWorkingMode(); else HandleSafetyMode();
    */
    char temp[] = "light";
    char temp1[] = "tv";
    device_relay.On(temp);
    delay(1000);
    device_relay.Off(temp);
    delay(1000);

    device_ir.UseIR(temp1);
}
// =======================================
