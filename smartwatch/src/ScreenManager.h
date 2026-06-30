#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Screen.h"

class ScreenManager{
    private:
        Screen** screens;
        int current;
        int maxScreens;
    public:
        ScreenManager(int maxsize){
            maxScreens = maxsize;
            screens = new Screen*[maxScreens];
            current = 0;
        }

        void initializeScreens(Screen** initScreens){
            for(int i=0;i<maxScreens;i++)
                screens[i]=initScreens[i];
        }

        void begin(){
            screens[current]->show();
        }

        void swipeLeft() {
            current = (current + 1) % maxScreens;  // go to next, wrap around
            screens[current]->show();
        }

        void swipeRight() {
            current = (current - 1 + maxScreens) % maxScreens;  // go to previous, wrap around
            screens[current]->show();
        }

        Screen* getCurrentScreen(){
            return screens[current];
        }

        ~ScreenManager(){
            delete[] screens;
        }
};

#endif