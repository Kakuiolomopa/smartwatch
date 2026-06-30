#ifndef APPICON_H
#define APPICON_H
#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>

// Forward declarations instead of includes
class App;
class MainMenuApp;

class AppIcon{
    private:
        lv_obj_t *btn;
        lv_obj_t *nameLabel;
        App* targetApp;
        MainMenuApp* manager;
        int id;
        
        static void app_clicked(lv_event_t *e);
    public:
        AppIcon(lv_obj_t *parent, const char* name, App* app, int id, MainMenuApp* manager, int col_pos,int col_span, int row_pos,int row_span){
            this->manager = manager;
            this->id = id;
            this->targetApp = app;
            btn = lv_btn_create(parent); 
            lv_obj_set_grid_cell(btn,LV_GRID_ALIGN_STRETCH,col_pos,col_span,LV_GRID_ALIGN_STRETCH,row_pos,row_span);
            lv_obj_add_event_cb(btn, app_clicked, LV_EVENT_CLICKED, this);
            lv_obj_set_style_pad_all(btn, 0, 0);   
            
            nameLabel = lv_label_create(btn);
            lv_label_set_text(nameLabel, name);
            lv_obj_center(nameLabel);
        }

        App* getApp(){
            return targetApp;
        }

        ~AppIcon(){
            delete targetApp;
        }
};

#include "MainMenuApp.h"

#endif