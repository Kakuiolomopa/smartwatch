#ifndef WATCHAPP_H
#define WATCHAPP_H

#include "App.h"
#include "WatchFaceScreen.h"
#include "TimerScreen.h"
#include "StopWatchScreen.h"
#include "AlarmScreen.h"


class WatchApp: public App{
    private:
        int maxsize;
        Screen** initScreens;
    public:
        WatchApp(int maxsize) : App(maxsize) {
            this->maxsize = maxsize;
            initScreens = new Screen*[maxsize];
            for(int i=0;i<maxsize;i++){
                initScreens[i] = nullptr;
            }
        }


        void create() override{
            if(initScreens[0]) {
                screenManager.begin();
                return;
            }
            initScreens[0] = new WatchFaceScreen();
            initScreens[1] = new TimerScreen();
            initScreens[2] = new StopWatchScreen();
            initScreens[3] = new AlarmScreen([this](bool v){change_run(v);});
            for(int i=0;i<maxsize;i++){
                initScreens[i]->create();
                lv_obj_add_event_cb(initScreens[i]->lvScreen,back_cb,LV_EVENT_GESTURE,this);
            }
            screenManager.initializeScreens(initScreens);
            screenManager.begin();
        }

        void update() override{
            for(int i=0;i<maxsize;i++)
                screenManager.getScreen(i)->update();
        }

        ~WatchApp() {
            for(int i = 0; i < maxsize; i++)
                delete initScreens[i];  // delete actual WatchFaceScreen objects
            delete[] initScreens;
        }
};

#endif