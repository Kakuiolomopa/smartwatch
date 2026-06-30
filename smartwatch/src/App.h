#ifndef APP_H
#define APP_H

#include "ScreenManager.h"
#include "Screen.h"

class App{
    protected:
        ScreenManager screenManager;

    public:
        App(int maxsize) : screenManager(maxsize){
        }

        virtual void create() = 0;
        virtual void update() = 0;

        virtual ~App(){}
        
};

#endif