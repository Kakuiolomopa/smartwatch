#ifndef WATCHAPP_H
#define WATCHAPP_H

#include "App.h"
#include "WatchFaceScreen.h"


class WatchApp: public App{
    private:
        int maxsize;
        Screen** initScreens;
    public:
        WatchApp(int maxsize) : App(maxsize) {
            this->maxsize = maxsize;
            initScreens = new Screen*[maxsize];
        }


        void create() override{
            for(int i=0;i<maxsize;i++){
                initScreens[i] = new WatchFaceScreen();
                initScreens[i]->create();
                lv_obj_add_event_cb(initScreens[i]->lvScreen,back_cb,LV_EVENT_GESTURE,this);
            }
            screenManager.initializeScreens(initScreens);
            screenManager.begin();
        }

        void update() override{
            Screen* active = screenManager.getCurrentScreen();
            active->update();
        }

        ~WatchApp() {
            for(int i = 0; i < maxsize; i++)
                delete initScreens[i];  // delete actual WatchFaceScreen objects
            delete[] initScreens;
        }
};

#endif