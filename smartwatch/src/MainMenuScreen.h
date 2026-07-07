#ifndef MAINMENUSCREEN_H
#define MAINMENUSCREEN_H

#include "Screen.h"
#include "AppIcon.h"
#include "App.h"
#include "WatchApp.h"

class MainMenuScreen : public Screen {
    private:
        lv_coord_t col_dsc[4];
        lv_coord_t row_dsc[4];
        AppIcon** appicons;
        App** apps;
        int appiconsize;
        App* activeapp;
    public:
        MainMenuScreen(int appiconsize) : Screen(){
            col_dsc[0] = LV_GRID_FR(1); col_dsc[1] = LV_GRID_FR(1); col_dsc[2] = LV_GRID_FR(1); col_dsc[3] = LV_GRID_TEMPLATE_LAST;
            row_dsc[0] = LV_GRID_FR(1); row_dsc[1] = LV_GRID_FR(1); row_dsc[2] = LV_GRID_FR(1); row_dsc[3] = LV_GRID_TEMPLATE_LAST;
            this->appiconsize = appiconsize;
            appicons = new AppIcon*[appiconsize]; 
            apps = new App*[appiconsize];
        }

        void create() override {

            lv_obj_set_style_grid_column_dsc_array(lvScreen, col_dsc, 0);
            lv_obj_set_style_grid_row_dsc_array(lvScreen, row_dsc, 0);
            lv_obj_set_size(lvScreen, LV_PCT(100),LV_PCT(100));
            lv_obj_center(lvScreen);
            lv_obj_set_layout(lvScreen, LV_LAYOUT_GRID);
            lv_obj_set_style_pad_all(lvScreen, 10, 0);
            lv_obj_set_style_pad_gap(lvScreen, 10, 0);

            int row=0;
            int column=0;

            for(int i=0;i<appiconsize;i++){
                apps[i] = new WatchApp(1);
                appicons[i] = new AppIcon(lvScreen,"Clock",[this,i](){startapp(apps[i]);},column,1,row,1);
                column++;
                if(column==2){
                    row++;
                    column=0;
                } 
            }
        }

        void update() override{
            if(activeapp){
                activeapp->update();
            }
        }

        void startapp(App* targetApp){
            activeapp = targetApp;
            targetApp->onBack = [this](){
                activeapp = nullptr;
                this->show();
            };
            targetApp->create();
        }

        ~MainMenuScreen(){
            for(int i=0;i<appiconsize;i++){
                delete apps[i];
                delete appicons[i];
            }
            delete[] apps;
            delete[] appicons;
        }
};

#endif
