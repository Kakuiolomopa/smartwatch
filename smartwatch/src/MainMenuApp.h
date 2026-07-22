#ifndef MAINMENUAPP_H
#define MAINMENUAPP_H

#include "Screen.h"
#include "MainMenuScreen.h"
#include "config.h"
#include <WiFi.h>

class MainMenuApp : public App{
    private:
        int menusize;
        Screen** menu;

        const char* ssid = WIFI_SSID;
        const char* password = WIFI_PASS;
        const char* tzInfo = "EET-2EEST,M3.5.0/3,M10.5.0/4";
        const char* ntpServer1 = "pool.ntp.org";
        const char* ntpServer2 = "time.nist.gov";

        void connectWiFi(){
            WiFi.mode(WIFI_STA);
            WiFi.begin(ssid, password);

            int attempts = 40;
            while (WiFi.status() != WL_CONNECTED && attempts > 0) {
                delay(250);
                attempts--;
            }
        }

        void syncTime(){
            configTime(0,0,ntpServer1,ntpServer2);

            setenv("TZ", tzInfo,1);
            tzset();

            time_t now = 0;
            int attempts = 20;
            while(time(&now) < 1577836800 && attempts>0){
                delay(250);
                attempts--;
            }
        }
    public:
        MainMenuApp(int menusize) : App(menusize){
            this->menusize = menusize;
            menu = new Screen*[menusize];
        }

        void create() override{
            connectWiFi();
            syncTime();
            int apps[3]={2,1,0};
            for(int i=0;i<menusize;i++){
                menu[i] = new MainMenuScreen(apps[i]);
                menu[i]->create();
            }
            screenManager.initializeScreens(menu);

            screenManager.begin();
        }

        void update() override{
            screenManager.getCurrentScreen()->update();
        }

        ~MainMenuApp(){
            for(int i=0;i<menusize;i++)
                delete menu[i];
            delete[] menu;  // safe, already nullptr
        }
};

#endif