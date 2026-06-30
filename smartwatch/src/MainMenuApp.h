#ifndef MAINMENUAPP_H
#define MAINMENUAPP_H

#include "AppIcon.h"
#include "Screen.h"
#include "WatchApp.h"
#include "MainMenuScreen.h"
#include "config.h"
#include <WiFi.h>

class MainMenuApp : public App{
    private:
        int appiconsize;
        int menusize;
        bool isHome;
        int workingAppId;
        Screen** menu;
        AppIcon** apps;

        const char* ssid = WIFI_SSID;
        const char* password = WIFI_PASS;
        const char* tzInfo = "EET-2EEST,M3.5.0/3,M10.5.0/4";
        const char* ntpServer1 = "pool.ntp.org";
        const char* ntpServer2 = "time.nist.gov";

        void connectWiFi(){
            WiFi.mode(WIFI_STA);
            WiFi.begin(ssid,password);

            while(WiFi.status() != WL_CONNECTED){
                delay(500);
            }
        }

        void syncTime(){
            configTime(0,0,ntpServer1,ntpServer2);

            setenv("TZ", tzInfo,1);
            tzset();

            time_t now = 0;
            while(time(&now) < 1577836800){
                delay(500);
            }
        }
    public:
        MainMenuApp(int menusize,int appiconsize) : App(menusize){
            workingAppId = -1;
            this->menusize = menusize;
            this->appiconsize = appiconsize;
            isHome = true;
            apps = new AppIcon*[appiconsize]; 
            menu = new Screen*[menusize];
        }

        void create(){
            connectWiFi();
            syncTime();
            for(int i=0;i<menusize;i++){
                menu[i] = new MainMenuScreen();
                menu[i]->create();
            }
            screenManager.initializeScreens(menu);

            apps[0] = new AppIcon(menu[0]->lvScreen,"Clock",new WatchApp(1),0,this,0,1,0,1);
            apps[1] = new AppIcon(menu[0]->lvScreen,"Clock",new WatchApp(1),1,this,1,1,0,1);

            delete[] menu;
            menu = nullptr;

            screenManager.begin();
        }

        void update(){
            if(!isHome){
                apps[workingAppId]->getApp()->update();
            }
        }

        void startapp(App* targetApp,int id){
            isHome = false;
            workingAppId = id;
            targetApp->create();
        }

        ~MainMenuApp(){
            for(int i = 0; i < appiconsize; i++)
                delete apps[i];  // delete each AppIcon (which deletes its targetApp)
            delete[] apps;
            delete[] menu;  // safe, already nullptr
        }
};

inline void AppIcon::app_clicked(lv_event_t *e){
    AppIcon* icon = (AppIcon*)lv_event_get_user_data(e);
    if(lv_event_get_code(e) == LV_EVENT_CLICKED){
        icon->manager->startapp(icon->targetApp, icon->id);
    }
}

#endif