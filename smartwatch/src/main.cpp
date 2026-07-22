#include <Arduino.h>
#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>
#include "MainMenuApp.h"

LilyGo_Class amoled;

MainMenuApp mainMenu(3);

void setup()
{
    disableCore0WDT();
    disableLoopWDT();

    Serial.begin(115200);
    delay(3000);

    amoled.beginAMOLED_191();
    amoled.setBrightness(200);
    beginLvglHelper(amoled);

    mainMenu.create();
}

void loop()
{
    mainMenu.update();
    lv_task_handler();
    delay(5);
}