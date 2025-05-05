#pragma once
#include "flexui.hpp"

/*
label* label1 = new label("label1", "hello world! this is clipping", "ClearSans-Regular.ttf", 40, 0, 10, 0, col(255, 255, 0));
panel* panel1 = new panel("panel1", 100, 100, 300, 300, 0);
panel1->set_bg_col(col(255, 0, 0));
panel1->add_widget(label1);
add_widget(panel1);
button* btn1 = new button("btn1", "click me!", 500, 300, 100, 50, 0);
add_widget(btn1);
*/
listbox* list1 = new listbox("editor_widget_list", 0,  50, 200, config.h - 50, 0);  
list1->set_bg(col(24));
list1->set_fg(col(255));
add_widget(list1);

