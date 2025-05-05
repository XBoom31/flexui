#include "flexui.hpp"

flexui ui;
sconfig config;

void widget::draw(){
};
void widget::init(SDL_Renderer* renderer){
};
void widget::handle_event(SDL_Event& event){
};



void flexui::initialize(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)  
        printf("SDL couldn't be initialized");
    
    win = SDL_CreateWindow(config.title, config.x, config.y, config.w, config.h, SDL_WINDOW_SHOWN);
    if(!win)
        printf("Failed to create window");

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer)
        printf("Failed to create renderer");
    
    TTF_Init();

    #include "widgets.hpp"
    for(auto w : widgets){
        w->init(renderer);
    }

}

void flexui::draw(){
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    for(int z = -10; z < 10; z++){
        for(auto w : widgets){
            if(w->zindex == z)
                w->draw();
        }
    }
    
    SDL_RenderPresent(renderer);
}

void flexui::run(){
    while(running){
        handle_event();
        draw();
    }
}

void flexui::assign(char id[256], std::function<void()> func, int type){
    for(auto w : all_widgets){
        if(!strcmp(w->id, id)){
           switch (type){
               case 0:
                w->on_click = func;
                break;
           }
        }
    }
}

void flexui::handle_event(){
    SDL_Event event;
    SDL_WaitEvent(&event);

    if(event.type == SDL_QUIT)
    {
        running = false;
    }

    for(auto w : widgets){
        w->handle_event(event);
    }
}

widget* flexui::get_widget(char id[256]){
     for(auto w : all_widgets){
        if(!strcmp(w->id, id)){
            return w;
        }
    }
     return 0;
}

void widget::move(int x1, int y1){
    x = x1;
    y = y1;
    init(renderer);
}

void label::init(SDL_Renderer* renderer){
    
    this->renderer = renderer;

    font = TTF_OpenFont(font_path, font_size);
    if (!font) {
        printf("Font error: %s\n", TTF_GetError());
        return;
    }

    SDL_Color textColor = { font_color.r, font_color.g, font_color.b, font_color.a };
    surf = TTF_RenderText_Solid(font, text, textColor);
    if (!surf) {
        printf("Surface error: %s\n", TTF_GetError());
        return;
    }

    tex = SDL_CreateTextureFromSurface(renderer, surf);
    if (!tex) {
        printf("Texture error: %s\n", SDL_GetError());
        return;
    }

    SDL_FreeSurface(surf); 

    w = render_rect.w = surf->w;
    h = render_rect.h = surf->h;
    render_rect.x = x;
    render_rect.y = y;
}

void label::draw(){
    SDL_RenderCopy(renderer, tex, NULL, &render_rect);
}
void label::handle_event(SDL_Event& event){
    if(event.type == SDL_MOUSEBUTTONDOWN){
        int mx = event.button.x, my = event.button.y;
        if(mx > render_rect.x & mx < render_rect.x + render_rect.w &&
                my > render_rect.y && my < render_rect.y + render_rect.h)
            if(on_click) on_click();
    } 
}

void panel::draw(){
    SDL_RenderSetClipRect(renderer, &render_rect);
    box(renderer, render_rect, FILLED, bg); 
    for(auto w : kids){
        w->draw();
    }
    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = config.w;
    r.h = config.h;
    SDL_RenderSetClipRect(renderer, &r);
}
void panel::init(SDL_Renderer* renderer){
    this->renderer = renderer;
    render_rect.x = x;
    render_rect.y = y;
    render_rect.w = w;
    render_rect.h = h;
    for(auto w : kids){
        w->init(renderer);
        w->move(x + w->x, y + w->y);
    }
}
void panel::handle_event(SDL_Event& event){
    for(auto w : kids)
        w->handle_event(event);
}
void panel::add_widget(widget* w){
    kids.push_back(w);
    ui.all_widgets.push_back(w); 
}

void button::init(SDL_Renderer* renderer){
    this->renderer = renderer;
    font = create_font("ClearSans-Regular.ttf", 10); 
}

void button::draw(){
    box(renderer, x, y, w, h, FILLED, bg);
    int a = 0, b = 0;
    text(renderer, label, font, text_color, x + w/2, y + h / 2, a, b, true, true);
}

void button::handle_event(SDL_Event& event){

}

void listbox::draw(){
    box(renderer, x, y, w, h, FILLED, bg); 
    for(int i = 0; i < list.size(); i++){
        box(renderer, x, y + i * item_h, w, item_h, FILLED, item);
        int a, b;
        text(renderer, list.at(i).label, font, text_color, x, y + i * item_h + item_h / 2, a, b, false, true); 
    }

}

void listbox::init(SDL_Renderer* renderer){
    this->renderer = renderer;
    font = create_font("ClearSans-Regular.ttf", 10); 
}

void listbox::handle_event(SDL_Event& event){

}


void listbox::add_item(char label[255], std::function<void()> func){
    list.push_back(list_item(label, func));
}

void select_item(char label[255]){


}

