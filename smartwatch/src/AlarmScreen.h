#ifndef ALARMSCREEN_H
#define ALARMSCREEN_H

#include "Screen.h"
#include <cstring>
#include <time.h>

class AlarmScreen : public Screen{
    private:
        lv_obj_t *add_btn;
        lv_obj_t *menu;
        lv_obj_t *hour_roller;
        lv_obj_t *minute_roller;
        lv_obj_t *ampm_roller;
        lv_obj_t *ok_btn;
        uint32_t place;

        lv_obj_t* main_page;

        lv_obj_t *alarm_box;

        int index;
        int cur_ind;

        lv_obj_t *edit_page;
        lv_obj_t* edit_cont;
        lv_obj_t* edit_time_label;
        lv_obj_t *alarm_name;
        lv_obj_t* kb;
        lv_obj_t* del_btn;
        lv_obj_t* save_btn;
        lv_obj_t* cb_btn[7];



        struct alarm_container{
            lv_obj_t *container,*time_label,*schedule_label,*onOff_switch;
            std::string alarm_message;
            bool weekdays[7];
            bool on,scheduled;
            int hour,minute,last_sound;
        };

        alarm_container alarms[10];

        struct tm timeinfo;

        std::function<void(bool)> change_run;

        void generate_alarm(char ampm[],int hour,int minute){
            lv_obj_t *container = lv_obj_create(lvScreen);
            lv_obj_set_size(container,LV_PCT(100),100);
            lv_obj_align(container,LV_ALIGN_TOP_MID,0,place);
            lv_obj_add_flag(container,LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_event_cb(container,edit,LV_EVENT_CLICKED,this);
            alarms[index].container = container;
            place+=100;
            lv_obj_t *time_label = lv_label_create(container);
            lv_obj_set_style_text_font(time_label,&lv_font_montserrat_38,0);
            char buf[6];
            snprintf(buf, sizeof(buf), "%02u:%02u", hour, minute);
            lv_label_set_text(time_label, buf);
            lv_obj_align(time_label,LV_ALIGN_LEFT_MID,5,10);

            alarms[index].time_label = time_label;

            lv_obj_t *schedule_label = lv_label_create(container);
            lv_obj_set_style_text_font(schedule_label,&lv_font_montserrat_22,0);
            lv_label_set_text(schedule_label,"Not scheduled");
            lv_obj_align(schedule_label,LV_ALIGN_LEFT_MID,5,-25);

            alarms[index].schedule_label = schedule_label;

            lv_obj_t *onOff_switch = lv_switch_create(container);
            lv_obj_set_size(onOff_switch,100,40);
            lv_obj_add_state(onOff_switch,LV_STATE_CHECKED);
            lv_obj_align(onOff_switch,LV_ALIGN_RIGHT_MID,-5,0);
            lv_obj_add_event_cb(onOff_switch,switch_state,LV_EVENT_VALUE_CHANGED,this);

            alarms[index].onOff_switch = onOff_switch;
            alarms[index].on = true;
            alarms[index].scheduled = false;
            alarms[index].last_sound = -1;
            for(int i=0;i<7;i++)
                alarms[index].weekdays[i] = false;
        }

        void openMenu(){
            lv_obj_clear_flag(menu, LV_OBJ_FLAG_HIDDEN);
            lv_obj_move_foreground(menu);
            change_run(false);
        }

        void closeMenu(){
            lv_obj_add_flag(menu,LV_OBJ_FLAG_HIDDEN);
            change_run(true);
        }

        void update_schedule_label(){
            std::string schedule_text = "";
            if(alarms[cur_ind].scheduled){
                for(int i=0;i<7;i++)
                    if(alarms[cur_ind].weekdays[i])
                        switch(i){
                            case 0: schedule_text += "Sun "; break;
                            case 1: schedule_text += "Mon "; break;
                            case 2: schedule_text += "Tue "; break;
                            case 3: schedule_text += "Wed "; break;
                            case 4: schedule_text += "Thu "; break;
                            case 5: schedule_text += "Fri "; break;
                            case 6: schedule_text += "Sat "; break;
                        }
            } else {
                schedule_text = "Not scheduled";
            }
            lv_label_set_text(alarms[cur_ind].schedule_label, schedule_text.c_str());
        }

        static void edit(lv_event_t *e){
            AlarmScreen *as = (AlarmScreen*)lv_event_get_user_data(e);
            lv_obj_t *cont = lv_event_get_target(e);
            for(int i=0;i<as->index;i++)
                if(as->alarms[i].container == cont){
                    as->cur_ind = i;
                    break;
                }
            char buf[6];
            snprintf(buf, sizeof(buf), "%02u:%02u", as->alarms[as->cur_ind].hour, as->alarms[as->cur_ind].minute);
            lv_label_set_text(as->edit_time_label, buf);
            lv_menu_set_page(as->menu,as->edit_page);
            lv_textarea_set_text(as->alarm_name,as->alarms[as->cur_ind].alarm_message.c_str());
            for(int i=0;i<7;i++)
                if(as->alarms[as->cur_ind].weekdays[i])
                    lv_obj_add_state(as->cb_btn[i],LV_STATE_CHECKED);
                else
                    lv_obj_clear_state(as->cb_btn[i],LV_STATE_CHECKED);
            as->openMenu();
        }

        static void save_edit(lv_event_t *e){
            AlarmScreen *as = (AlarmScreen*)lv_event_get_user_data(e);
            as->alarms[as->cur_ind].scheduled = false;
            for(int i=0;i<7;i++)
                if(lv_obj_has_state(as->cb_btn[i],LV_STATE_CHECKED)){
                    as->alarms[as->cur_ind].weekdays[i] = true;
                    as->alarms[as->cur_ind].scheduled = true;
                }else
                    as->alarms[as->cur_ind].weekdays[i] = false;
            as->alarms[as->cur_ind].alarm_message = lv_textarea_get_text(as->alarm_name);
            as->update_schedule_label();
            as->closeMenu();
        }

        static void del_alarm(lv_event_t *e){
            AlarmScreen *as = (AlarmScreen*)lv_event_get_user_data(e);
            lv_obj_del(as->alarms[as->cur_ind].container);
            for(int i=0;i<7;i++)
                lv_obj_clear_state(as->cb_btn[i],LV_STATE_CHECKED);
            for(int i=as->cur_ind; i<as->index-1; i++){
                as->alarms[i].container = as->alarms[i+1].container;
                as->alarms[i].time_label = as->alarms[i+1].time_label;
                as->alarms[i].schedule_label = as->alarms[i+1].schedule_label;
                as->alarms[i].onOff_switch = as->alarms[i+1].onOff_switch;
                as->alarms[i].alarm_message = as->alarms[i+1].alarm_message;
                for(int j=0;j<7;j++)
                    as->alarms[i].weekdays[j] = as->alarms[i+1].weekdays[j];
                as->alarms[i].on = as->alarms[i+1].on;
                as->alarms[i].scheduled = as->alarms[i+1].scheduled;
                as->alarms[i].hour = as->alarms[i+1].hour;
                as->alarms[i].minute = as->alarms[i+1].minute;  
            }
            as->index--;
            as->place=40;
            for(int i=0;i<as->index;i++){
                lv_obj_align(as->alarms[i].container,LV_ALIGN_TOP_MID,0,as->place);
                as->place+=100;
            }
            as->closeMenu();
        }

        static void switch_state(lv_event_t *e){
            AlarmScreen *as = (AlarmScreen*)lv_event_get_user_data(e);
            lv_obj_t* sw = lv_event_get_target(e);
            for(int i=0;i<as->index;i++)
                if(as->alarms[i].onOff_switch == sw){
                    as->alarms[i].on = lv_obj_has_state(sw,LV_STATE_CHECKED);
                    break;
                }          
        }

        static void show_menu(lv_event_t *e){
            AlarmScreen *as = (AlarmScreen*)lv_event_get_user_data(e);
            lv_menu_set_page(as->menu,as->main_page);
            as->openMenu();
        }

        static void hide_menu(lv_event_t *e){
            AlarmScreen *as = (AlarmScreen*)lv_event_get_user_data(e);
            as->closeMenu();
        }

        static void create_alarm(lv_event_t* e){
            AlarmScreen* as = (AlarmScreen*)lv_event_get_user_data(e);
            if(as->index == 10){
                lv_obj_t* limit_box = lv_msgbox_create(as->lvScreen,"Alarm limit reached","You reached your limit of 10 alarms. To add a new alarm please delete any of the existing alarms.",NULL,true);
                lv_obj_set_size(limit_box,LV_PCT(75),LV_PCT(65));
                lv_obj_move_foreground(limit_box);
                lv_obj_center(limit_box);
                as->closeMenu();
                return;
            }
            char buf[4];
            lv_roller_get_selected_str(as->ampm_roller,buf,sizeof(buf));
            if(buf[0]=='A')
                as->alarms[as->index].hour=(lv_roller_get_selected(as->hour_roller)+1)%12;
            else
                as->alarms[as->index].hour=(lv_roller_get_selected(as->hour_roller)+1)%12+12;
            as->alarms[as->index].minute=lv_roller_get_selected(as->minute_roller)+1;
            as->generate_alarm(buf,as->alarms[as->index].hour,as->alarms[as->index].minute);
            as->alarms[as->index].alarm_message = std::string("");
            as->index++;
            as->closeMenu();
        }

        static void focus(lv_event_t *e){
            AlarmScreen *as = (AlarmScreen*)lv_event_get_user_data(e);
            lv_event_code_t code = lv_event_get_code(e);
            if(code == LV_EVENT_CLICKED){
                lv_obj_align(as->menu,LV_ALIGN_TOP_MID,0,-60);
                lv_obj_scroll_to(as->edit_cont, 0, lv_obj_get_y(as->alarm_name) - 20, LV_ANIM_OFF);
                lv_obj_clear_flag(as->edit_cont,LV_OBJ_FLAG_SCROLLABLE);
                lv_obj_clear_flag(as->kb,LV_OBJ_FLAG_HIDDEN);
                lv_obj_move_foreground(as->kb);
            }else if(code == LV_EVENT_READY){
                lv_obj_add_flag(as->kb,LV_OBJ_FLAG_HIDDEN);
                lv_obj_center(as->menu);
                lv_obj_add_flag(as->edit_cont,LV_OBJ_FLAG_SCROLLABLE);
            }
        }

        static void stop_alarm(lv_event_t *e){
            AlarmScreen *as = (AlarmScreen*)lv_event_get_user_data(e);
            if(as->alarm_box){
                lv_obj_del_async(as->alarm_box);
                as->alarm_box = nullptr;
            }
        }
    public:
        AlarmScreen(std::function<void(bool)> change_run):Screen(){
            index=0;
            this->change_run = change_run;
            place=0;
            cur_ind = 0;
            alarm_box = nullptr;
        }

        void create() override{
            lv_obj_set_size(lvScreen,LV_PCT(100),LV_PCT(100));
            lv_obj_center(lvScreen);

            add_btn = lv_btn_create(lvScreen);
            lv_obj_set_size(add_btn,LV_PCT(100),30);
            lv_obj_add_event_cb(add_btn,show_menu,LV_EVENT_CLICKED,this);
            lv_obj_align(add_btn,LV_ALIGN_TOP_MID,0,5);
            place+=40;

            lv_obj_t *label = lv_label_create(add_btn);
            lv_label_set_text(label,"Add");
            lv_obj_center(label);


            menu = lv_menu_create(lvScreen);
            lv_obj_set_size(menu,LV_PCT(75),LV_PCT(65));
            lv_menu_set_mode_root_back_btn(menu,LV_MENU_ROOT_BACK_BTN_ENABLED);
            lv_obj_add_event_cb(menu,hide_menu,LV_EVENT_CLICKED,this);
            lv_obj_center(menu);
            lv_obj_add_flag(menu,LV_OBJ_FLAG_HIDDEN); 
            lv_obj_add_flag(menu,LV_OBJ_FLAG_FLOATING);
            

            main_page = lv_menu_page_create(menu,NULL);
            lv_obj_t* cont = lv_obj_create(main_page);
            lv_obj_set_size(cont,LV_PCT(100),LV_PCT(100));
            lv_menu_set_page(menu,main_page);

            lv_obj_t *cancel_btn = lv_menu_get_main_header_back_btn(menu);
            lv_obj_t *cancel_label = lv_label_create(cancel_btn);
            lv_label_set_text(cancel_label,"Cancel");

            ampm_roller = lv_roller_create(cont);
            lv_roller_set_options(ampm_roller,
                                    "AM\n"
                                    "PM",
                                    LV_ROLLER_MODE_NORMAL);
            lv_roller_set_visible_row_count(ampm_roller,1);
            lv_obj_set_size(ampm_roller,70,LV_PCT(80));
            lv_obj_set_style_text_font(ampm_roller,&lv_font_montserrat_18,0);
            lv_obj_set_style_text_align(ampm_roller,LV_TEXT_ALIGN_CENTER,0);
            lv_obj_align(ampm_roller,LV_ALIGN_LEFT_MID,5,0);
            lv_roller_set_selected(ampm_roller,0,LV_ANIM_ON);

            hour_roller = lv_roller_create(cont);
            lv_roller_set_options(hour_roller,
                                    "1\n"
                                    "2\n"
                                    "3\n"
                                    "4\n"
                                    "5\n"
                                    "6\n"
                                    "7\n"
                                    "8\n"
                                    "9\n"
                                    "10\n"
                                    "11\n"
                                    "12",
                                    LV_ROLLER_MODE_NORMAL);
            lv_roller_set_visible_row_count(hour_roller,3);
            lv_obj_set_size(hour_roller,90,LV_PCT(100));
            lv_obj_set_style_text_font(hour_roller,&lv_font_montserrat_22,0);
            lv_obj_set_style_text_align(hour_roller,LV_TEXT_ALIGN_CENTER,0);
            lv_obj_align(hour_roller,LV_ALIGN_CENTER,-50,0);
            lv_roller_set_selected(hour_roller,0,LV_ANIM_ON);

            lv_obj_t* lb = lv_label_create(cont);
            lv_obj_set_style_text_font(lb,&lv_font_montserrat_40,0);
            lv_obj_center(lb);
            lv_label_set_text(lb,":");

            minute_roller = lv_roller_create(cont);
            lv_roller_set_options(minute_roller,
                                    "1\n"
                                    "2\n"
                                    "3\n"
                                    "4\n"
                                    "5\n"
                                    "6\n"
                                    "7\n"
                                    "8\n"
                                    "9\n"
                                    "10\n"
                                    "11\n"
                                    "12\n"
                                    "13\n"
                                    "14\n"
                                    "15\n"
                                    "16\n"
                                    "17\n"
                                    "18\n"
                                    "19\n"
                                    "20\n"
                                    "21\n"
                                    "22\n"
                                    "23\n"
                                    "24\n"
                                    "25\n"
                                    "26\n"
                                    "27\n"
                                    "28\n"
                                    "29\n"
                                    "30\n"
                                    "31\n"
                                    "32\n"
                                    "33\n"
                                    "34\n"
                                    "35\n"
                                    "36\n"
                                    "37\n"
                                    "38\n"
                                    "39\n"
                                    "40\n"
                                    "41\n"
                                    "42\n"
                                    "43\n"
                                    "44\n"
                                    "45\n"
                                    "46\n"
                                    "47\n"
                                    "48\n"
                                    "49\n"
                                    "50\n"
                                    "51\n"
                                    "52\n"
                                    "53\n"
                                    "54\n"
                                    "55\n"
                                    "56\n"
                                    "57\n"
                                    "58\n"
                                    "59",
                                    LV_ROLLER_MODE_NORMAL);
            lv_roller_set_visible_row_count(minute_roller,3);
            lv_obj_set_size(minute_roller,90,LV_PCT(100));
            lv_obj_set_style_text_font(minute_roller,&lv_font_montserrat_22,0);
            lv_obj_set_style_text_align(minute_roller,LV_TEXT_ALIGN_CENTER,0);
            lv_obj_align(minute_roller,LV_ALIGN_CENTER,50,0);
            lv_roller_set_selected(minute_roller,0,LV_ANIM_ON);

            ok_btn = lv_btn_create(cont);
            lv_obj_add_event_cb(ok_btn,create_alarm,LV_EVENT_CLICKED,this);
            lv_obj_set_size(ok_btn,50,40);
            lv_obj_align(ok_btn,LV_ALIGN_BOTTOM_RIGHT,-5,-5);

            lb = lv_label_create(ok_btn);
            lv_label_set_text(lb,"Ok");
            lv_obj_center(lb);

            edit_page = lv_menu_page_create(menu,NULL);
            edit_cont = lv_obj_create(edit_page);
            lv_obj_set_size(edit_cont,LV_PCT(100),LV_PCT(100));
            lv_menu_set_page(menu,edit_page);

            edit_time_label = lv_label_create(edit_cont);
            lv_label_set_text(edit_time_label,"00:00");
            lv_obj_set_style_text_font(edit_time_label,&lv_font_montserrat_30,0);
            lv_obj_align(edit_time_label,LV_ALIGN_TOP_LEFT,5,0);
            
            lv_obj_t *cb_label;
            const char *days[] = {"S","M","T","W","T","F","S"};
            for(int i=0;i<7;i++){
                cb_btn[i] = lv_btn_create(edit_cont);
                lv_obj_set_size(cb_btn[i],45,35);
                lv_obj_align(cb_btn[i],LV_ALIGN_TOP_LEFT,5+i*51,35);
                lv_obj_add_flag(cb_btn[i],LV_OBJ_FLAG_CHECKABLE);

                cb_label = lv_label_create(cb_btn[i]);
                lv_obj_set_style_text_font(cb_label,&lv_font_montserrat_20,0);
                lv_label_set_text(cb_label,days[i]);
                lv_obj_center(cb_label);
            }

            alarm_name = lv_textarea_create(edit_cont);
            lv_textarea_set_one_line(alarm_name,true);
            lv_textarea_set_password_mode(alarm_name,false);
            lv_obj_set_width(alarm_name,LV_PCT(50));
            lv_obj_add_event_cb(alarm_name,focus,LV_EVENT_ALL,this);
            lv_obj_align(alarm_name,LV_ALIGN_TOP_MID,40,80);

            lv_obj_t *oneline_label = lv_label_create(edit_cont);
            lv_label_set_text(oneline_label,"Alarm name:");
            lv_obj_align_to(oneline_label,alarm_name,LV_ALIGN_OUT_LEFT_MID,0,0);

            kb = lv_keyboard_create(lvScreen);
            lv_obj_set_size(kb,LV_HOR_RES,LV_PCT(70));
            lv_keyboard_set_textarea(kb,alarm_name);
            lv_obj_add_flag(kb,LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(kb,LV_OBJ_FLAG_FLOATING);

            lv_obj_t *temp_label;
            del_btn = lv_btn_create(edit_cont);
            lv_obj_set_size(del_btn,60,45);
            lv_obj_align(del_btn,LV_ALIGN_BOTTOM_LEFT,3,95);
            lv_obj_add_event_cb(del_btn,del_alarm,LV_EVENT_CLICKED,this);

            static lv_style_t style_red_outline;
            lv_style_init(&style_red_outline);
            lv_style_set_border_width(&style_red_outline, 2);
            lv_style_set_border_color(&style_red_outline, lv_color_make(255,0,0));
            lv_style_set_border_opa(&style_red_outline, LV_OPA_COVER);
            lv_style_set_bg_opa(&style_red_outline, LV_OPA_TRANSP);
            lv_style_set_text_color(&style_red_outline, lv_color_make(255,0,0));
            lv_style_set_shadow_width(&style_red_outline, 20);
            lv_style_set_shadow_color(&style_red_outline, lv_color_make(255,0,0));
            lv_style_set_shadow_opa(&style_red_outline, LV_OPA_50);
            lv_style_set_shadow_ofs_x(&style_red_outline, 0);
            lv_style_set_shadow_ofs_y(&style_red_outline, 0);
            lv_obj_add_style(del_btn, &style_red_outline, 0);
            
            temp_label = lv_label_create(del_btn);
            lv_label_set_text(temp_label,"Delete");
            lv_obj_center(temp_label);

            save_btn = lv_btn_create(edit_cont);
            lv_obj_set_size(save_btn,60,45);
            lv_obj_align(save_btn,LV_ALIGN_BOTTOM_RIGHT,-3,95);
            lv_obj_add_event_cb(save_btn,save_edit,LV_EVENT_CLICKED,this);

            temp_label = lv_label_create(save_btn);
            lv_label_set_text(temp_label,"Save");
            lv_obj_center(temp_label);
        } 

        void update() override{
            if(getLocalTime(&timeinfo, 0)){
                getLocalTime(&timeinfo,2000);
                for(int i=0;i<index;i++){
                    if(alarms[i].on){
                        if(timeinfo.tm_hour == alarms[i].hour && timeinfo.tm_min == alarms[i].minute){
                            if(!alarms[i].scheduled){
                                closeMenu();
                                alarms[i].on = false;
                                lv_obj_clear_state(alarms[i].onOff_switch,LV_STATE_CHECKED);
                                static const char *btns[] = {"Stop Alarm",NULL};
                                alarm_box = lv_msgbox_create(lvScreen,"Alarm!!!!",alarms[i].alarm_message.c_str(),btns,false);
                                lv_obj_set_size(alarm_box,LV_PCT(75),LV_PCT(65));
                                lv_obj_center(alarm_box);
                                lv_obj_add_event_cb(alarm_box,stop_alarm,LV_EVENT_VALUE_CHANGED,this);
                                lv_obj_move_foreground(alarm_box);
                                lv_obj_t *txt = lv_msgbox_get_text(alarm_box);
                                lv_obj_set_style_text_font(txt,&lv_font_montserrat_28,0);

                                lv_obj_set_style_flex_main_place(alarm_box,LV_FLEX_ALIGN_CENTER,0);
                                lv_obj_set_flex_align(alarm_box,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER);
                            }else
                                if(alarms[i].last_sound != timeinfo.tm_wday && alarms[i].weekdays[timeinfo.tm_wday]){
                                    closeMenu();
                                    static const char *btns[] = {"Stop Alarm",NULL};
                                    alarm_box = lv_msgbox_create(lvScreen,"Alarm!!!!",alarms[i].alarm_message.c_str(),btns,false);
                                    lv_obj_set_size(alarm_box,LV_PCT(75),LV_PCT(65));
                                    lv_obj_center(alarm_box);
                                    lv_obj_add_event_cb(alarm_box,stop_alarm,LV_EVENT_VALUE_CHANGED,this);
                                    lv_obj_move_foreground(alarm_box);

                                    lv_obj_set_flex_align(alarm_box,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER);
                                    alarms[i].last_sound = timeinfo.tm_wday;
                                }
                        }
                    }
                }
            }
        }

        ~AlarmScreen(){
        }

};

#endif