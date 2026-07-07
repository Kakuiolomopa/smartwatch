#ifndef SCREEN_H
#define SCREEN_H

#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>

class Screen {
    public:
        lv_obj_t *lvScreen;

        Screen(){
            lvScreen = lv_obj_create(NULL);
        }

        virtual void create() = 0;
        virtual void update() = 0;

        void show() {
            lv_scr_load_anim(lvScreen, LV_SCR_LOAD_ANIM_FADE_IN, 300, 0, false);
        }

        void swipeRight(){
            lv_scr_load_anim(lvScreen,LV_SCR_LOAD_ANIM_MOVE_RIGHT,300,0,false);
        }

        void swipeLeft(){
            lv_scr_load_anim(lvScreen,LV_SCR_LOAD_ANIM_MOVE_LEFT,300,0,false);
        }

        void hide() {
        }

        virtual ~Screen(){}
};

#endif