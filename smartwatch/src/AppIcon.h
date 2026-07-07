#ifndef APPICON_H
#define APPICON_H
#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>

class AppIcon{
    private:
        lv_obj_t *btn;
        lv_obj_t *nameLabel;
        std::function<void()> onClick;
    public:
        AppIcon(lv_obj_t *parent, const char* name, std::function<void()> callback, int col_pos,int col_span, int row_pos,int row_span):onClick(callback){
            btn = lv_btn_create(parent); 
            lv_obj_set_grid_cell(btn,LV_GRID_ALIGN_STRETCH,col_pos,col_span,LV_GRID_ALIGN_STRETCH,row_pos,row_span);
            lv_obj_add_event_cb(btn, app_clicked, LV_EVENT_CLICKED, this);
            lv_obj_set_style_pad_all(btn, 0, 0);   
            
            nameLabel = lv_label_create(btn);
            lv_label_set_text(nameLabel, name);
            lv_obj_center(nameLabel);
        }

        static void app_clicked(lv_event_t *e) {
            AppIcon* icon = (AppIcon*)lv_event_get_user_data(e);
            if (icon->onClick) icon->onClick();
        }
};

#endif