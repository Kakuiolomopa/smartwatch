#ifndef MAINMENUSCREEN_H
#define MAINMENUSCREEN_H

#include "Screen.h"

class MainMenuScreen : public Screen {
    private:
        static lv_coord_t col_dsc[4];
        static lv_coord_t row_dsc[4];
    public:
        MainMenuScreen() : Screen(){
            col_dsc[0] = LV_GRID_FR(1); col_dsc[1] = LV_GRID_FR(1); col_dsc[2] = LV_GRID_FR(1); col_dsc[3] = LV_GRID_TEMPLATE_LAST;
            row_dsc[0] = LV_GRID_FR(1); row_dsc[1] = LV_GRID_FR(1); row_dsc[2] = LV_GRID_FR(1); row_dsc[3] = LV_GRID_TEMPLATE_LAST;
        }

        void create() {

            lv_obj_set_style_grid_column_dsc_array(lvScreen, col_dsc, 0);
            lv_obj_set_style_grid_row_dsc_array(lvScreen, row_dsc, 0);
            lv_obj_set_size(lvScreen, LV_PCT(100),LV_PCT(100));
            lv_obj_center(lvScreen);
            lv_obj_set_layout(lvScreen, LV_LAYOUT_GRID);
            lv_obj_set_style_pad_all(lvScreen, 10, 0);
            lv_obj_set_style_pad_gap(lvScreen, 10, 0);
        }

        void update(){
        }
};

#endif
