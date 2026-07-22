#ifndef STOPWATCHSCREEN_H
#define STOPWATCHSCREEN_H

#include "Screen.h"

class StopWatchScreen : public Screen{
    private:
        lv_obj_t *st_label;
        lv_obj_t *start_btn;
        lv_obj_t *reset_btn;
        lv_obj_t *start_label;
        lv_style_t style_btn;
        bool isStarted;

        uint8_t seconds;
        uint8_t minutes;
        uint32_t hours;

        void writeTime(){
            char buf[16];
            snprintf(buf, sizeof(buf), "%02u:%02u:%02u", hours, minutes, seconds);
            lv_label_set_text(st_label, buf);
        }

        static void startStop_time(lv_event_t *e){
            StopWatchScreen *st = (StopWatchScreen*)lv_event_get_user_data(e);
            st->isStarted = !st->isStarted;
            if(st->isStarted){
                lv_obj_add_style(st->start_btn,&st->style_btn,0);
                lv_label_set_text(st->start_label,"Stop");
            }else{
                lv_obj_remove_style(st->start_btn,&st->style_btn,0);
                lv_label_set_text(st->start_label,"Start");
            }
        }

        static void reset_time(lv_event_t *e){
            StopWatchScreen *st = (StopWatchScreen*)lv_event_get_user_data(e);
            st->isStarted = false;
            st->seconds=0;
            st->minutes=0;
            st->hours=0;
            lv_obj_remove_style(st->start_btn,&st->style_btn,0);
            lv_label_set_text(st->start_label,"Start");
            st->writeTime();
        }
    public:
        StopWatchScreen() : Screen(){
            isStarted = false;
            seconds=0;
            minutes=0;
            hours=0;
            lv_style_init(&style_btn);
            lv_style_set_bg_color(&style_btn,lv_color_make(255,0,0));
            lv_style_set_bg_opa(&style_btn,LV_OPA_COVER);
        }

        void create() override{
            lv_obj_set_size(lvScreen,LV_PCT(100),LV_PCT(100));
            lv_obj_center(lvScreen);
            lv_obj_set_flex_flow(lvScreen,LV_FLEX_FLOW_COLUMN);
            lv_obj_set_flex_align(lvScreen,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER);
            
            st_label = lv_label_create(lvScreen);
            lv_obj_set_style_text_font(st_label,&lv_font_montserrat_40,0);
            writeTime();


            start_btn = lv_btn_create(lvScreen);
            lv_obj_set_size(start_btn,200,50);
            lv_obj_add_event_cb(start_btn,startStop_time,LV_EVENT_CLICKED,this);
            start_label = lv_label_create(start_btn);
            lv_label_set_text(start_label,"Start");
            lv_obj_center(start_label);

            reset_btn = lv_btn_create(lvScreen);
            lv_obj_set_size(reset_btn,200,50);
            lv_obj_add_event_cb(reset_btn,reset_time,LV_EVENT_CLICKED,this);
            lv_obj_t *btn_label = lv_label_create(reset_btn);
            lv_label_set_text(btn_label,"Reset");
            lv_obj_center(btn_label);
        }

        void update() override{
            if(isStarted){
                static uint32_t lastTick;
                if(millis()-lastTick>=1000){
                    lastTick = millis();
                    seconds++;
                    if(seconds==60){
                        minutes++;
                        seconds=0;
                    }
                    if(minutes==60){
                        hours++;
                        minutes=0;
                        seconds=0;
                    }
                    writeTime();
                }
            }
        }

        ~StopWatchScreen(){

        }
};

#endif