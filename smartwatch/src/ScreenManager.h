#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Screen.h"

class ScreenManager{
    private:
        Screen** screens;
        int current;
        int maxScreens;

        static void swipe(lv_event_t *e){
            ScreenManager *sm = (ScreenManager*)lv_event_get_user_data(e);
            lv_indev_t *indev = lv_event_get_indev(e);
            lv_dir_t dir = lv_indev_get_gesture_dir(indev);

            switch(dir){
                case LV_DIR_LEFT:
                    sm->swipeRight();
                    break;
                case LV_DIR_RIGHT:
                    sm->swipeLeft();
                    break;
            }
        }
    public:
        ScreenManager(int maxsize){
            maxScreens = maxsize;
            screens = new Screen*[maxScreens];
            current = 0;
        }

        void initializeScreens(Screen** initScreens){
            for(int i=0;i<maxScreens;i++){
                screens[i]=initScreens[i];
                lv_obj_add_event_cb(screens[i]->lvScreen,swipe,LV_EVENT_GESTURE,this);
            }
        }

        void begin(){
            screens[current]->show();
        }

        void swipeLeft() {
            current = (current - 1 + maxScreens) % maxScreens;  // go to next, wrap around
            screens[current]->swipeRight();
        }

        void swipeRight() {
            current = (current + 1) % maxScreens;  // go to previous, wrap around
            screens[current]->swipeLeft();
        }

        Screen* getCurrentScreen(){
            return screens[current];
        }

        Screen* getScreen(int index){
            return screens[index];
        }

        ~ScreenManager(){
            delete[] screens;
        }
};

#endif