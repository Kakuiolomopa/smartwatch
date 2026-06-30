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

        void create(){
            for(int i=0;i<maxsize;i++){
                initScreens[i] = new WatchFaceScreen();
                initScreens[i]->create();
            }
            screenManager.initializeScreens(initScreens);
            screenManager.begin();
        }

        void update(){
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