#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "flexconfig.hpp"
#include <functional>

#define OUTLINE 0
#define FILLED 1


enum flex_signals{
    ON_CLICK,
    ON_HOVER
};
class col{

    public:
        int  r;
        int g;
        int b;
        int a;
        col(char r1, char b1, char c1){
            r = r1;
            g = b1;
            b = c1;
            a = 255;
        };
        col(int c){
            r = c; g = c; b = c; a = 255;
        };
        col(){
          r = 0; g = 0; b= 0; a=255;
        }
};

inline TTF_Font* create_font(char path[256], int size){
    auto f = TTF_OpenFont(path, size);
    if(!f){
        printf("Failed to create font: %s\n", TTF_GetError());
        return NULL;
    }
    return f;
}

inline void box(SDL_Renderer* renderer, int x, int y, int w, int h, bool filled, col c){
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    if(filled)
        SDL_RenderFillRect(renderer, &r);
}    
inline void box(SDL_Renderer* renderer, SDL_Rect r, bool filled, col c){
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    if(filled)
        SDL_RenderFillRect(renderer, &r);
}    
//optimize later
inline void text(SDL_Renderer* renderer, char text[256], TTF_Font* font, col c, int x, int y, int &w, int &h, bool centerx = false, bool centery = false){
    SDL_Color col = {c.r, c.g, c.b, c.a};
    SDL_Surface* surf = TTF_RenderText_Solid(font, text, col);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    w = surf->w;
    h = surf->h;
    SDL_FreeSurface(surf);
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    if(centerx){
        r.x -= r.w / 2;
    }
    if(centery){
        r.y -= r.h / 2;
    }
    SDL_RenderCopy(renderer, tex, NULL, &r);
}


class widget{
public:
    std::function<void()> on_click;
    char id[256];
    bool visible;
    SDL_Renderer* renderer;
    int x, y, w, h;
    void move(int x1, int y1);
    virtual void draw();
    virtual void init(SDL_Renderer* renderer);
    virtual void handle_event(SDL_Event& event);
    int zindex;
    SDL_Rect render_rect; 
};

class label : public widget{
public:
    label(char label1[256], char txt[256], char fontp[256], int font_sz,int x1, int y1, int z, col color){
        strcpy(text, txt);
        strcpy(id, label1);
        strcpy(font_path, fontp);
        font_size = font_sz;
        zindex = z;
        x = x1;
        y = y1;
        font_color = color;
    }
    col font_color;
    int font_size;
    char text[256];
    char font_path[256];
    void draw() override;
    void init(SDL_Renderer* renderer) override;
    void handle_event(SDL_Event& event)override;
    TTF_Font* font;
    SDL_Surface* surf;
    SDL_Texture* tex;
};

class panel : public widget{
public:
    panel(char label[256], int x1, int y1, int w1, int h1, int z){
        strcpy(id, label);
        x = x1;
        y = y1;
        w = w1;
        h = h1;
        zindex = z;
    }
    void set_bg_col(col c){
        bg = c;
    }
    col bg = col(255);
    void draw() override;
    void init(SDL_Renderer* renderer) override;
    void handle_event(SDL_Event& event) override;
    void add_widget(widget* w);
    std::vector<widget*> kids;

};

class button : public widget{
    public:
    char label[256];
    col bg = col(200);
    col text_color = col(0);

    TTF_Font* font;
    button(char label[256], char text[256], int x1, int y1, int w1, int h1, int z){
        strcpy(id, label);
        strcpy(this->label, text);
        x = x1;
        y = y1;
        w = w1;
        h = h1;
    }
    void draw() override;
    void init(SDL_Renderer* renderer) override;
    void handle_event(SDL_Event& event) override;
 
};

class list_item{
    public:
    list_item(char l[255], std::function<void()> func){
        strcpy(label, l);
        onclick = func;
    }
    char label[255];
    std::function<void()> onclick;

};


class listbox : public widget{
public:
    listbox(char label[255], int x1, int y1, int w1, int h1, int z){
        strcpy(id, label);
        x = x1;
        y = y1;
        w = w1;
        h = h1;
        zindex = z;
    }
    std::vector<list_item> list;
    col bg;
    col item;
    col selected;
    col text_color;
    col selected_text_color;
    int item_h = 20;
    TTF_Font* font;
    int selected_item;
    void draw() override;
    void init(SDL_Renderer* renderer) override;
    void handle_event(SDL_Event& event) override;
    void add_item(char  label[255], std::function<void()> func);
    void select_item(char label[255]);
    void set_bg(col c){
        bg = c;
    }
    void set_fg(col c){
        text_color = c;
    }
};

class flexui{
    std::vector<widget*> widgets;
    void initialize();
    void handle_event();
    void draw();
    bool running = true;
    SDL_Window* win;
    SDL_Renderer* renderer;
    public:
    std::vector<widget*> all_widgets;
    void add_widget(widget* w){
        widgets.push_back(w);
        all_widgets.push_back(w);
    }
    widget* get_widget(char id[256]);
    void assign(char id[256], std::function<void()> func, int type);
    flexui(){
        initialize();
    }
    void run();
};
extern flexui ui;
