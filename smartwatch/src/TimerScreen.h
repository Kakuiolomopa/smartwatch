#ifndef TIMERSCREEN_H
#define TIMERSCREEN_H

#include "Screen.h"

class TimerScreen: public Screen{
    private:
        lv_obj_t *timer_label;
        uint32_t time;
        lv_obj_t *start_btn;
        lv_obj_t *start_label;
        lv_style_t style_btn;
        lv_coord_t col_dsc[4];
        lv_coord_t row_dsc[7];
        bool isStarted;
        bool isTurned;

        struct BtnConfig{
            const char* label;
            uint8_t value;
            uint8_t multiplier;
        };

        struct BtnData{
            TimerScreen *screen;
            uint8_t value;
            uint8_t multiplier;
        };

        BtnData *btnData[11];
        
        void turnIntoDateTime(){
            uint8_t seconds,minutes,hours,carry;
            if(!isTurned){
                seconds = time%100;
                time /= 100;
                carry = seconds/60;
                seconds %= 60;
                minutes = time%100+carry;
                time /= 100;
                carry = minutes/60;
                minutes %= 60;
                hours = time+carry;
            }else{
                hours = time / 3600;
                time %= 3600;
                minutes = time / 60;
                time %= 60;
                seconds = time;
            }
            time = hours*3600+minutes*60+seconds;
            isTurned = true;

            char buf[16];
            snprintf(buf, sizeof(buf), "%02u:%02u:%02u", hours, minutes, seconds);
            lv_label_set_text(timer_label, buf);
        }

        void normalizeText(){
            uint32_t time_copy = time;
            uint8_t seconds = time_copy%100;
            time_copy/=100;
            uint8_t minutes = time_copy%100;
            time_copy/=100;
            uint8_t hours = time_copy%100;

            char buf[16];
            snprintf(buf, sizeof(buf), "%02u:%02u:%02u", hours, minutes, seconds);
            lv_label_set_text(timer_label, buf);
        }

        static void add_time(lv_event_t *e){
            BtnData* d = (BtnData*)lv_event_get_user_data(e);
            if(d->screen->time*d->multiplier+d->value<1000000 && !d->screen->isStarted && !d->screen->isTurned){
                d->screen->time=d->screen->time*d->multiplier+d->value;
                d->screen->normalizeText();
            }
        }

        static void delete_time(lv_event_t *e){
            TimerScreen* tm = (TimerScreen*)lv_event_get_user_data(e);
            if(!tm->isStarted && tm->isTurned){
                tm->time = 0;
                tm->isTurned = false;
                tm->normalizeText();
            }else if(!tm->isStarted){
                tm->time /= 10;
                tm->normalizeText();
            }
        }

        static void start_time(lv_event_t *e){
            TimerScreen* tm = (TimerScreen*)lv_event_get_user_data(e);
            tm->isStarted = !tm->isStarted;
            tm->turnIntoDateTime();
            if(tm->isStarted){
                lv_obj_add_style(tm->start_btn,&tm->style_btn,0);

                lv_label_set_text(tm->start_label,"STOP");
            }else{
                lv_obj_remove_style(tm->start_btn, &tm->style_btn, 0);

                lv_label_set_text(tm->start_label,"START");
            }
        }
    public:
       TimerScreen():   Screen(){
            time = 0;
            isStarted = false;
            isTurned = false;
            col_dsc[0] = LV_GRID_FR(1); col_dsc[1] = LV_GRID_FR(1); col_dsc[2] = LV_GRID_FR(1); col_dsc[3] = LV_GRID_TEMPLATE_LAST;
            row_dsc[0] = LV_GRID_FR(1); row_dsc[1] = LV_GRID_FR(1); row_dsc[2] = LV_GRID_FR(1); row_dsc[3] = LV_GRID_FR(1); row_dsc[4] = LV_GRID_FR(1); row_dsc[5] = LV_GRID_FR(1); row_dsc[6] = LV_GRID_TEMPLATE_LAST;
            lv_style_init(&style_btn);
            lv_style_set_bg_color(&style_btn,lv_color_make(255,0,0));
            lv_style_set_bg_opa(&style_btn,LV_OPA_COVER);
        }

       void create() override{
            lv_obj_set_style_grid_column_dsc_array(lvScreen,col_dsc,0);
            lv_obj_set_style_grid_row_dsc_array(lvScreen,row_dsc,0);
            lv_obj_set_size(lvScreen,LV_PCT(100),LV_PCT(100));
            lv_obj_center(lvScreen);
            lv_obj_set_layout(lvScreen,LV_LAYOUT_GRID);

            timer_label = lv_label_create(lvScreen);
            lv_obj_set_grid_cell(timer_label,LV_GRID_ALIGN_SPACE_EVENLY,1,3,LV_GRID_ALIGN_SPACE_EVENLY,0,1);
            lv_obj_set_style_text_font(timer_label,&lv_font_montserrat_28,0);
            normalizeText();

            lv_obj_t *timer_btn;
            lv_obj_t *btn_label;

            uint16_t row=1;
            uint16_t col=0;

            BtnConfig btns[] = {
                {"1",1,10}, {"2",2,10}, {"3",3,10},
                {"4",4,10}, {"5",5,10}, {"6",6,10},
                {"7",7,10}, {"8",8,10}, {"9",9,10},
                {"0",0,10}, {"00",0,100}
            };

            for(uint16_t i=0;i<11;i++){

                timer_btn = lv_btn_create(lvScreen);
                lv_obj_set_grid_cell(timer_btn,LV_GRID_ALIGN_STRETCH,col,1,LV_GRID_ALIGN_STRETCH,row,1);
                btnData[i] = new BtnData{this,btns[i].value,btns[i].multiplier};
                lv_obj_add_event_cb(timer_btn,add_time,LV_EVENT_CLICKED,btnData[i]);

                btn_label = lv_label_create(timer_btn);
                lv_label_set_text(btn_label,btns[i].label);
                lv_obj_center(btn_label);
                
                col++;
                if(col>=3){
                    col=0;
                    row++;
                }
            }
            timer_btn = lv_btn_create(lvScreen);
            lv_obj_set_grid_cell(timer_btn,LV_GRID_ALIGN_STRETCH,col,1,LV_GRID_ALIGN_STRETCH,row,1);
            lv_obj_add_event_cb(timer_btn,delete_time,LV_EVENT_CLICKED,this);

            btn_label = lv_label_create(timer_btn);
            lv_label_set_text(btn_label,"DEL");
            lv_obj_center(btn_label);

            col = 1;
            row++;

            start_btn = lv_btn_create(lvScreen);
            lv_obj_set_grid_cell(start_btn,LV_GRID_ALIGN_STRETCH,col,1,LV_GRID_ALIGN_STRETCH,row,1);
            lv_obj_add_event_cb(start_btn,start_time,LV_EVENT_CLICKED,this);

            start_label = lv_label_create(start_btn);
            lv_label_set_text(start_label,"START");
            lv_obj_center(start_label);
       }

       void update() override{
            if(isStarted){
                if(time!=0){
                    static uint32_t lastTick;
                    if(millis() - lastTick>=1000){
                        time--;
                        lastTick = millis();
                        turnIntoDateTime();
                    }
                }else{
                    isStarted = false;
                    isTurned = false;
                    lv_obj_remove_style(start_btn, &style_btn, 0);
                    lv_label_set_text(start_label,"START");
                }
            }
       }

       ~TimerScreen(){
            for(int i=0;i<11;i++){
                delete btnData[i];
            }
       }

};

#endif