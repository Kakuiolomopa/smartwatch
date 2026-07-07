#ifndef APP_H
#define APP_H

#include "ScreenManager.h"
#include "Screen.h"

class App{
    protected:
        ScreenManager screenManager;

    public:

        std::function<void()> onBack;

        static void back_cb(lv_event_t *e){
            App* app = (App*)lv_event_get_user_data(e);
            lv_indev_t* indev = lv_event_get_indev(e);
            if(lv_indev_get_gesture_dir(indev)==LV_DIR_BOTTOM && app->onBack){
                app->onBack();
            }
        }

        App(int maxsize) : screenManager(maxsize){
        }

        virtual void create() = 0;
        virtual void update() = 0;

        virtual ~App(){}
        
};

#endif