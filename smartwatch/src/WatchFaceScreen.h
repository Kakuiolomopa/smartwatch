#ifndef WATCHFACESCREEN_H
#define WATCHFACESCREEN_H

#include "Screen.h"
#include <time.h>

class WatchFaceScreen: public Screen{
    private:
        lv_obj_t *timeLabel;
        struct tm timeinfo;

        char* printDateTime() {
            static char formattedTime[64];  // static buffer, 64 chars of space
            
            if (!getLocalTime(&timeinfo, 2000)) {
                strcpy(formattedTime, "00:00");
                return formattedTime;
            }
            
            strftime(formattedTime, sizeof(formattedTime), "%A, %B %d %Y %H:%M:%S", &timeinfo);
            return formattedTime;
        }
    public:
        WatchFaceScreen() : Screen(){}

        void create() override{
            timeLabel = lv_label_create(lvScreen);
            lv_obj_set_style_text_font(timeLabel,&lv_font_montserrat_28,0);

            lv_label_set_text(timeLabel, printDateTime());
            

            lv_obj_center(timeLabel);
        }

        void update() override{
            if(getLocalTime(&timeinfo, 0)){
                lv_label_set_text(timeLabel,printDateTime());
            }
        }
};

#endif