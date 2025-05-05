#include "flexui.hpp"


void clicked_on_label(){
    printf("clicked");
    widget* w = ui.get_widget("label1");
    w->move(300, 300);
}

void selected_label(){

}

void selected_button(){

}


void populate_widget_list(){
    listbox* list = (listbox*)ui.get_widget("editor_widget_list");
    list->add_item("Label", selected_label);
    list->add_item("Button", selected_button);
}

void setup_editor(){
    populate_widget_list();    
}

int main(){
    ui.assign("label1", clicked_on_label, ON_CLICK);
    setup_editor();
    ui.run();
    return 0;
}
