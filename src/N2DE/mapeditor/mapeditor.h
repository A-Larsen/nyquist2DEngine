/* 
 * Copyright (C) 2022  Austin Larsen
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef N2DE_MAPEDITOR_H_
#define N2DE_MAPEDITOR_H_
// TODO:
// create different tool types
// - move tool
// - add tool
// - select tool

// TODO
// have zoom zoom in where mouse cursor is

#include "../../lua5.4/lua.h"
#include "../../lua5.4/lauxlib.h"
#include "../../lua5.4/lualib.h"
#include "../declare.h"
#include "../memory.h"
#include "../sprites.h"
#include "../lines.h"
#include "../rects.h"
#include "../world.h"
#include "../fonts.h"
#include "../config.h"
#include "../mouse.h"
#include "./sdl.h"
#include <stdint.h>

#define SDL_main main

bool WORLD_UPDATE[9] = {
    [0] = true, 
    [1] = false, 
    [2] = false, 
    [3] = false, 
    [4] = false,
    [5] = false,
    [6] = false,
    [7] = false,
    [8] = false,
};
float WORLD_SCALE[9] = {
    [0] = 1, 
    [1] = 1, 
    [2] = 1, 
    [3] = 1, 
    [4] = 1,
    [5] = 1,
    [6] = 1,
    [7] = 1,
    [8] = 1,
};

#define OBJECT_F_VISIBLE (1 << 0)
#define OBJECT_F_NOTVISIBLE (~(1 << 0))

#pragma GCC diagnostic ignored "-Wunused-variable"
#define MAPEDITOR_DESTRUCTOR(mapeditor) \
    Sprites *sprites = &mapeditor->sprites; \
    SDLData *sdlData = &mapeditor->sdlData; \
    Images *images = &mapeditor->images; \
    Fonts *fonts = &mapeditor->fonts; \
    Object *objects = mapeditor->objects; \
    TypeAndCrop *typeAndCrops = mapeditor->typeAndCrops; \
    ObjectSelector *objectSelector = &mapeditor->objectSelector; \
    WorldInfo *worldInfo = &mapeditor->worldInfo; \
    Lines *lines = &mapeditor->lines; \
    Mouse *mouse = &mapeditor->mouse; \
    SDL_Event *event = &mapeditor->event; \

typedef struct _TypeAndCrop {
    char type[50];
    SDL_Point position;
    SDL_Rect crops[CROP_VARIATION_COUNT]; 
    bool is_collidable;
    bool is_visable;
    SDL_Rect hitbox;
} TypeAndCrop;



enum COLORS {
    COLOR_BLACK ,
    COLOR_GREY,
    COLOR_SELECTOR_BACKGROUND
};

static SDL_Color mapeditor_colors[] = {
    [COLOR_BLACK] = {.r = 0, .g = 0, .b = 0, .a = 255},
    [COLOR_GREY] = {.r = 230, .g = 230, .b = 230, .a = 255},
    [COLOR_SELECTOR_BACKGROUND] = {.r = 230, .g = 230, .b = 230, .a = 200}
};

typedef struct _ObjectSelector {
    int width;
    int default_width;
    SDL_Color background_color;
    int padding;
    int font_id;
    int offset;
    bool fullscreen_toggle;
    uint8_t background_rect_id;
    uint8_t name_rect_id;
    uint8_t line_id;
} ObjectSelector;

#define WORLDINFO_COUNT 6
typedef struct _WorldInfo {
    /* 0 background_id; */
    /* 1 spritesheet_id; */
    /* 2 ttf_id; */
    /* 3 world_name_id; */
    /* 4 objects_count_id; */

    /* uint8_t grid_id; */
    uint8_t ids[WORLDINFO_COUNT];
    uint8_t position;
} WorldInfo;

#define TOGGLE_OBJECTSELECT (1 << 0)
#define TOGGLE_INFO (1 << 1)

typedef enum _Modes {
    MODE_PLACE,
    MODE_SELECT,
    MODE_MOVE,
    MODE_MUTLISELECT,
    MODE_SIZE
} Modes;

typedef struct _MapEditor {
    char ttf_file[50];
    char spritesheet_file[50];
    char world_name[50];
    char *objectSelctor_selected;
    char background_image_file[50];
    uint8_t background_id;
    uint8_t font_size;
    WorldInfo worldInfo;
    uint16_t grid;
    Sprites sprites;
    SDLData sdlData;
    int spritesheet_id;
    // TODO:
    // make objects a double pointer so you can draw in layers
    Object *objects;
    TypeAndCrop *typeAndCrops;
    uint16_t typeAndCrops_count;
    uint16_t objects_count;
    uint8_t scale;
    Size size;
    float zoom;
    lua_State *lua_state;
    ControlsKV controls[20];
    int controls_length;
    SDL_Event event;
    bool quit;
    SDL_Point absolute_position;
    SDL_Point grid_position;
    World world;
    ObjectSelector objectSelector;
    Lines lines;
    Images images;
    Fonts fonts;
    Mouse mouse;
    uint8_t toggleFlags;
    float zoom_size;
    float zoom_default;
    ScreenElement *update_elements[MAX_UPDATE_ELEMENTS];
    uint16_t update_elements_count;
    bool object_seleted;
    uint16_t selected_object_id;
    uint8_t mode;
    uint8_t map_id;
    float objectSelector_scale;
} MapEditor;

static int
quantize(float n, float q) {
    float offset = n / q;
    float decimal = ((float)floor(offset) - offset);
    return  n + (q * decimal);
}

static bool _parse_combo(char *alias, char keyComb[2][50])
{
    int j = 0;
    int k = 0;

    int len = strlen(alias);

    bool hasSpace = false;
    for(int i = 0; i < len; ++i) {
        char c = alias[i];

        if (c == ' ') {
            hasSpace = true;
            continue;
        }
    }

    if (!hasSpace) {
        keyComb[0][0] = '\0';
        j++;
    }

    for(int i = 0; i < len; ++i) {
        char c = alias[i];

        if (c == ' ') {
            keyComb[j][k + 1] = '\0';
            j++;
            k = 0;
            continue;
        }
        keyComb[j][k] = c;
        k++;

    }
    keyComb[j][k] = '\0';
    return keyComb[0][0] != '\0';
}


void mapeditor_readConfigFile(MapEditor *mapeditor, char *file)
{
    mapeditor->lua_state = luaL_newstate();
    luaL_openlibs(mapeditor->lua_state);
    lua_State *L = mapeditor->lua_state;
    if (luaL_dofile(L, file) != LUA_OK) {
        N2DE_ERROR("Error reading cofig\n");
        luaL_error(L, lua_tostring(L, -1));
        exit(1);
    }

    lua_getglobal(L, "keys");

    if (!lua_istable(L, -1))  return;

    int i = 0;

    lua_pushnil(L);
    while(lua_next(L, -2) != 0) {
        ControlsKV *control = &mapeditor->controls[i];
        char *key = (char *)lua_tostring(L, -1);
        char *alias = (char *)lua_tostring(L, -2);

        int len = strlen(key);
        memset(control->key, 0, len + 1);
        memcpy(control->key, key, len);

        len = strlen(alias);
        memset(control->alias, 0, len + 1);
        memcpy(control->alias, alias, len);
        lua_pop(L, 1);
        i++;
    }
    mapeditor->controls_length = i;


    lua_getglobal(L, "world_name");
    char *world_name = (char *)lua_tostring(L, -1);
    memset(mapeditor->world_name, 0, 50);
    memcpy(mapeditor->world_name, world_name, strlen(world_name));

    lua_getglobal(L, "spritesheet");
    char *spritesheet = (char *)lua_tostring(L, -1);
    memset(mapeditor->spritesheet_file, 0, 50);
    memcpy(mapeditor->spritesheet_file, spritesheet, strlen(spritesheet));


    lua_getglobal(L, "map");

    lua_pushnil(L);
    while(lua_next(L, -2) != 0) {

        char *name = (char *)lua_tostring(L, -2);

        if (!strcmp(name, "grid")) {
            int grid = (int)lua_tonumber(L, -1);
            mapeditor->grid = grid;
        }

        if (!strcmp(name, "width")) {
            int width = (int)lua_tonumber(L, -1);
            mapeditor->size.w = width;
        }

        if (!strcmp(name, "height")) {
            int height = (int)lua_tonumber(L, -1);
            mapeditor->size.h = height;
        }

        lua_pop(L, 1);
    }

    lua_getglobal(L, "objects");

    lua_pushnil(L);
    i = 0;

    while(lua_next(L, -2) != 0) {
        i++;
        lua_pop(L, 1);
    }

    MEMRES(mapeditor->typeAndCrops, sizeof(TypeAndCrop) * i);
    TypeAndCrop *typeAndCrops = mapeditor->typeAndCrops;
    mapeditor->typeAndCrops_count = i;

    i = 0;
    lua_pushnil(L);
    while(lua_next(L, -2) != 0) {

        char *object_type = (char *)lua_tostring(L, -2);
        memset(typeAndCrops[i].type, 0, 50);
        memcpy(typeAndCrops[i].type, object_type, strlen(object_type));

        lua_pushnil(L);
        while(lua_next(L, -2)) { // type
            char *name = (char *)lua_tostring(L, -2);

            if (!strcmp(name, "crops")) {

                lua_pushnil(L);
                while(lua_next(L, -2)) { // crops
                
                    int crop_idx = (int)lua_tonumber(L, -2) - 1;

                    lua_pushnil(L);

                    while(lua_next(L, -2)) { // frame
                        char *cord = (char *)lua_tostring(L, -2);
                        int num = (int)lua_tonumber(L, -1);
                        if (!strcmp(cord, "x")) {
                            typeAndCrops[i].crops[crop_idx].x = num;
                        }
                        if (!strcmp(cord, "y")) {
                            typeAndCrops[i].crops[crop_idx].y = num;

                        }
                        if (!strcmp(cord, "w")) {
                            typeAndCrops[i].crops[crop_idx].w = num;

                        }
                        if (!strcmp(cord, "h")) {
                            typeAndCrops[i].crops[crop_idx].h = num;

                        }

                        lua_pop(L, 1);
                    }

                    lua_pop(L, 1);
                }

            }
            if (!strcmp(name, "hitbox")) {

                lua_pushnil(L);
                while(lua_next(L, -2)) {
                
                        char *cord = (char *)lua_tostring(L, -2);
                        int num = (int)lua_tonumber(L, -1);
                        if (!strcmp(cord, "x")) {
                            /* printf("hitbox x: %d\n", num); */
                            typeAndCrops[i].hitbox.x = num;
                        }
                        if (!strcmp(cord, "y")) {
                            
                            /* printf("hitbox y: %d\n", num); */
                            typeAndCrops[i].hitbox.y = num;

                        }
                        if (!strcmp(cord, "w")) {
                            
                            /* printf("hitbox w: %d\n", num); */
                            typeAndCrops[i].hitbox.w = num;

                        }
                        if (!strcmp(cord, "h")) {

                            /* printf("hitbox h: %d\n", num); */
                            typeAndCrops[i].hitbox.h = num;

                        }

                        lua_pop(L, 1);
                }

            }
            if (!strcmp(name, "is_collidable")) {

                bool is_collidable = lua_toboolean(L, -1);
                typeAndCrops[i].is_collidable = is_collidable;
            }

            if (!strcmp(name, "is_visable")) {
                bool is_visable = lua_toboolean(L, -1);
                typeAndCrops[i].is_visable = is_visable;
            }
            lua_pop(L, 1);
        }

        i++;
        lua_pop(L, 1);
    }
    /* lua_getglobal(L, "background_image"); */

    /* char *background_file = (char *)lua_tostring(L, -1); */
    /* memset(mapeditor->background_image_file, 0, 50); */
    /* memcpy(mapeditor->background_image_file, background_file, */
    /*        strlen(background_file)); */

    lua_close(mapeditor->lua_state);
}

SDL_Rect *mapeditor_findCrops(MapEditor *mapeditor, char *type)
{
    TypeAndCrop *typeAndCrops = mapeditor->typeAndCrops;
    for (uint16_t i = 0; i < mapeditor->typeAndCrops_count; ++i) {
        if (!strcmp(typeAndCrops[i].type, type)) {
            return typeAndCrops[i].crops;
        }
    }
    return NULL;
}

TypeAndCrop *mapeditor_findTypeAndCrops(MapEditor *mapeditor, char *type)
{
    TypeAndCrop *typeAndCrops = mapeditor->typeAndCrops;
    for (uint16_t i = 0; i < mapeditor->typeAndCrops_count; ++i) {
        if (!strcmp(typeAndCrops[i].type, type)) {
            return &typeAndCrops[i];
        }
    }
    return NULL;
}

void mapeditor_addObject(MapEditor *mapeditor, char *type, SDL_Point *position,
                         Size *size, uint8_t flags)
{
    MAPEDITOR_DESTRUCTOR(mapeditor);
    uint16_t i = mapeditor->objects_count++;

    char info[255];
    sprintf(info, "object count: %d", mapeditor->objects_count - 
                                     mapeditor->typeAndCrops_count);
    fonts_change(fonts, worldInfo->ids[4],
                 sdlData->renderer, info, mapeditor_colors[COLOR_GREY],
                 mapeditor_colors[COLOR_BLACK]);

    int spritesheet_id = mapeditor->spritesheet_id;

    /* SDL_Rect *crops = mapeditor_findCrops(mapeditor, type); */
    TypeAndCrop *typeAndCrop = mapeditor_findTypeAndCrops(mapeditor, type);
    SDL_Rect *crops = typeAndCrop->crops;
    if (!crops) {
        N2DE_ERROR("could not find type\n");
        exit(1);
    }

    MEMRES(mapeditor->objects, sizeof(Object) * mapeditor->objects_count);
    Object *object = &mapeditor->objects[i];
    memset(object, 0, sizeof(Object));

    bool add_to_world = false;
    if (flags & OBJECT_F_NOTVISIBLE) {
        object->crop_id = sprites_addCrops(sprites, sdlData->renderer, spritesheet_id, type, crops,
                                           CROP_VARIATION_COUNT, 4);
        add_to_world = false;
    } else {
        object->crop_id = sprites_addCrops(sprites, sdlData->renderer, spritesheet_id, type, crops,
                                           CROP_VARIATION_COUNT, 2);
        add_to_world = true;
    }

    int len = strlen(type);
    memset(object->type, 0, len + 1);
    memcpy(object->type, type, len);
    memcpy(&object->position, position, sizeof(SDL_Point));
    object->is_collidable = typeAndCrop->is_collidable;
    memcpy(&object->hitbox, &typeAndCrop->hitbox, sizeof(SDL_Rect));
    /* printf("hitbox: %d, %d, %d, %d\n", typeAndCrop->hitbox.x, typeAndCrop->hitbox.y, typeAndCrop->hitbox.w, typeAndCrop->hitbox.h); */
    object->is_visable = typeAndCrop->is_visable;
    memcpy(&object->size, size, sizeof(Size));
    /* printf("realtive size: %d, %d \n", size->w, size->h); */
    /* printf("realtive position: %d, %d \n", position->x, position->y); */
    memcpy(&object->crops, crops, sizeof(SDL_Rect) * CROP_VARIATION_COUNT);

    /* if (add_to_world) { */
    /*     world_addObject(&mapeditor->world, object); */
    /* } */
}

void mapeditor_init(MapEditor *mapeditor, char *file, const char *background_image, int map_id)
{
    if (TTF_Init() == -1) {
        N2DE_ERROR(TTF_GetError());
        exit(1);
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        N2DE_ERROR(IMG_GetError());
        exit(1);
    }
    memset(mapeditor, 0, sizeof(MapEditor));
    memset(mapeditor->background_image_file, 0, 50);
    memcpy(mapeditor->background_image_file, background_image, strlen(background_image));

    MAPEDITOR_DESTRUCTOR(mapeditor);
    mapeditor->update_elements_count = 0;
    objectSelector->offset = 0;
    worldInfo->position = 0;
    char path[255];
    mapeditor->font_size = 8;

    mapeditor_readConfigFile(mapeditor, file);
    mapeditor->toggleFlags = 0;
    mapeditor->zoom_size = 1.0f / (float)mapeditor->grid;
    mapeditor->zoom = 1.0f / ((float)mapeditor->grid);
    mapeditor->zoom_default = 1.0f / ((float)mapeditor->grid);
    mapeditor->scale = 5;
    images_init(images);
    mapeditor->size.w *= mapeditor->scale;
    mapeditor->size.h *= mapeditor->scale;
    Size sceen_size = {
        16 * 90,
        9 * 90
    };
    sdl_init(sdlData, &sceen_size, 1);
    lines_init(lines);
    rects_init();
    sprites_init(sprites);
    fonts_init(fonts);
    sprintf(path, RESPATH "/images/%s", mapeditor->spritesheet_file);
    mapeditor->spritesheet_id = sprites_new(sprites, sdlData->renderer, path);
    sprintf(path, RESPATH "/images/%s", mapeditor->background_image_file);
    mapeditor->background_id = images_create(images, sdlData->renderer, path,
                                             NULL, 0);

    mapeditor->objects_count = 0;
    mapeditor->objects = NULL;
    mapeditor->quit = false;

    objectSelector->padding = 2;
    objectSelector->fullscreen_toggle = false;
    objectSelector->default_width = 50 * mapeditor->grid;
    objectSelector->width = 50 * mapeditor->grid;
    objectSelector->background_rect_id = rects_create(&mapeditor_colors[COLOR_SELECTOR_BACKGROUND], true, 2);
    objectSelector->name_rect_id = rects_create(&mapeditor_colors[COLOR_GREY], true, 2);
    objectSelector->line_id = lines_create(lines, &mapeditor_colors[COLOR_BLACK], 8);

    memcpy(&objectSelector->background_color, &mapeditor_colors[COLOR_SELECTOR_BACKGROUND],
          sizeof(SDL_Color));

    memset(mapeditor->ttf_file, 0, 50);
    const char *ttf_file = "NotoSansMono-Regular.ttf";
    memcpy(mapeditor->ttf_file, ttf_file, strlen(ttf_file));
    sprintf(path, RESPATH "/fonts/%s", mapeditor->ttf_file);
    mapeditor->font_size *= mapeditor->scale;

    objectSelector->font_id = \
    fonts_create(fonts, path, mapeditor->font_size, sdlData->renderer, 
                 "Object Selector", 5, mapeditor_colors[COLOR_BLACK],
                 mapeditor_colors[COLOR_GREY]);

    char world_path[255];
    sprintf(world_path, RESPATH "/data/%s/", mapeditor->world_name);

    char info[255];
    sprintf(info, "spritesheet: %s", mapeditor->spritesheet_file);
    worldInfo->ids[1] = \
        fonts_create(fonts, path, mapeditor->font_size, sdlData->renderer, 
                     info, 5, mapeditor_colors[COLOR_GREY],
                     mapeditor_colors[COLOR_BLACK]);

    sprintf(info, "background: %s", mapeditor->background_image_file);

    worldInfo->ids[0] = \
        fonts_create(fonts, path, mapeditor->font_size, sdlData->renderer, 
                     info, 5, mapeditor_colors[COLOR_GREY],
                     mapeditor_colors[COLOR_BLACK]);

    sprintf(info, "ttf: %s", mapeditor->ttf_file);

    worldInfo->ids[2] = \
        fonts_create(fonts, path, mapeditor->font_size, sdlData->renderer, 
                     info, 5, mapeditor_colors[COLOR_GREY],
                     mapeditor_colors[COLOR_BLACK]);

    sprintf(info, "world name: %s", mapeditor->world_name);

    worldInfo->ids[3] = \
        fonts_create(fonts, path, mapeditor->font_size, sdlData->renderer, 
                     info, 5, mapeditor_colors[COLOR_GREY],
                     mapeditor_colors[COLOR_BLACK]);

    sprintf(info, "object count: %d", 0);

    worldInfo->ids[4] = \
        fonts_create(fonts, path, mapeditor->font_size, sdlData->renderer, 
                     info, 0, mapeditor_colors[COLOR_GREY],
                     mapeditor_colors[COLOR_BLACK]);

    sprintf(info, "grid: %d", mapeditor->grid);

    worldInfo->ids[5] = \
        fonts_create(fonts, path, mapeditor->font_size, sdlData->renderer, 
                     info, 5, mapeditor_colors[COLOR_GREY],
                     mapeditor_colors[COLOR_BLACK]);


    world_init(&mapeditor->world, world_path, mapeditor->spritesheet_id);
    /* world_getObjects(&mapeditor->world, 1, mapeditor->objects, &mapeditor->objects_count); */

    // You must add all of the objets before you can use draw them (duh..)
    for (uint16_t i = 0; i < mapeditor->typeAndCrops_count; ++i) {
        char * type = mapeditor->typeAndCrops[i].type;
        SDL_Point p1 = { -255, -255 };
        Size s1 = {1 , 1};
        mapeditor_addObject(mapeditor, type, &p1, &s1, OBJECT_F_NOTVISIBLE);
    }

    mapeditor->map_id = map_id; 
    world_read(&mapeditor->world, mapeditor->map_id, sdlData->renderer, images, sprites, 2);
 
    if (mapeditor->world.map_exists && mapeditor->world.objects_exists) {
        mapeditor->objects_count += mapeditor->world.map.objects_count;
        MEMRES(mapeditor->objects, sizeof(Object) * mapeditor->objects_count);
        uint16_t copy_size = sizeof(Object) * mapeditor->world.map.objects_count;
        memcpy(mapeditor->objects + mapeditor->typeAndCrops_count , mapeditor->world.objects, copy_size);

    }
    /* free(mapeditor->world.objects); */
    /* mapeditor->world.objects = NULL; */
    mapeditor->world.map.objects_count = 0;

    mapeditor->objectSelctor_selected = mapeditor->typeAndCrops[0].type;
    mapeditor->mode = MODE_PLACE;
    mapeditor->object_seleted = false;
    mapeditor->objectSelector_scale = .3;
}


void mapeditor_quit(MapEditor *mapeditor)
{
    sprites_free(&mapeditor->sprites);
    free(mapeditor->objects);
    world_free(&mapeditor->world);
    lines_free(&mapeditor->lines);
    rects_free();
    fonts_quit(&mapeditor->fonts);
    images_quit(&mapeditor->images);
    free(mapeditor->typeAndCrops);
    sdl_quit(&mapeditor->sdlData);
    TTF_Quit();
    IMG_Quit();
}

void mapeditor_clear(MapEditor *mapeditor)
{
    SDLData *sdlData = &mapeditor->sdlData;
    SDL_SetRenderDrawColor(sdlData->renderer, 0, 0, 0, 0);
    SDL_RenderClear(sdlData->renderer);
}

// Has a responsize design
void mapeditor_objectSelectorUpdate(MapEditor *mapeditor)
{
    MAPEDITOR_DESTRUCTOR(mapeditor);
    int spritesheet_id = mapeditor->spritesheet_id;

    objectSelector->width = objectSelector->fullscreen_toggle ?
                                        sdlData->size.w :
                                        objectSelector->default_width;

    int start_x = sdlData->size.w - objectSelector->width;

    SDL_Point left_border_start = {start_x, 0};
    SDL_Point left_border_end = {start_x, sdlData->size.h};

    /* lines_create(lines, &mapeditor_colors[COLOR_BLACK], &left_border_start, */
    /*              &left_border_end); */
    lines_que(lines, objectSelector->line_id, &left_border_start, &left_border_end);

    SDL_Rect *font_rect = fonts_getRect(fonts, objectSelector->font_id);

    int content_start_x = start_x + objectSelector->padding;
    int content_start_y = font_rect->h + objectSelector->padding;

    content_start_y += objectSelector->offset;
    int x = content_start_x;
    int y = content_start_y;

    int tallest = 0;

    SDL_Rect background_rect = {
        start_x,
        0,
        objectSelector->width,
        sdlData->size.h,
    };

    /* rects_create(rects, &objectSelector->background_color, &background_rect, true); */
    uint8_t rect_id = 0;
    rects_que(objectSelector->background_rect_id, &background_rect);

    for (uint16_t i = 0; i < mapeditor->typeAndCrops_count; ++i) {
        SDL_Rect *crop = &typeAndCrops[i].crops[0];
        SDL_Point *pos = &typeAndCrops[i].position;
        char *type = typeAndCrops[i].type;
        SDL_Rect *next_crop = &typeAndCrops[i + 1].crops[0];
        int w = crop->w * mapeditor->grid * mapeditor->objectSelector_scale;
        int h = crop->h * mapeditor->grid * mapeditor->objectSelector_scale;

        if (h > tallest) tallest = h;

        /* SDL_Point position = {x, y}; */

        Size size = {w , h};
        Size scale = {1 , 1};
        pos->x = x;
        pos->y = y;
        sprites_updateCrop(sprites, sdlData->renderer, *pos, size, scale, 0,
                           spritesheet_id, type);

        x += w + objectSelector->padding;

        if (i + 1 < mapeditor->typeAndCrops_count) {
            int w = next_crop->w * mapeditor->grid * mapeditor->objectSelector_scale;
            int h = next_crop->h * mapeditor->grid * mapeditor->objectSelector_scale;
            int nx = x + w + objectSelector->padding;
            if (nx > sdlData->size.w) {
                x = content_start_x;
                y += objectSelector->padding + tallest;
                tallest = 0;
            }

        }
        /* if (x + w + objectSelector->padding > sdlData->size.w) { */
        /*     x = content_start_x; */
        /*     y += objectSelector->padding + tallest; */
        /*     tallest = 0; */
        /* } */
        sprites_que(sprites, mapeditor->spritesheet_id, pos);
    }
    /* rects_update(rects, sdlData->renderer); */
    /* lines_update(lines, sdlData->renderer); */
    /* sprites_updateQue(sprites, mapeditor->update_elements, &mapeditor->update_elements_count); */

    int center_xpadding = (objectSelector->width - font_rect->w) / 2;

    // center the font
    SDL_Point fontPos = {
         sdlData->size.w - objectSelector->width + center_xpadding,
         0
    };

    SDL_Rect rect = {
        content_start_x,
        0,
        objectSelector->width,
        font_rect->h + 2,
    };

    /* rects_create(rects, &objectSelector->background_color, &rect, true); */
    rects_que(objectSelector->name_rect_id, &rect);
    SDL_Point p1 = {start_x, rect.h};
    SDL_Point p2 = {content_start_x + objectSelector->width, rect.h};

    /* lines_create(lines, &mapeditor_colors[COLOR_BLACK], &p1, &p2); */
    lines_que(lines, objectSelector->line_id, &p1, &p2);
    /* rects_update(rects, sdlData->renderer); */
    /* lines_update(lines, sdlData->renderer); */
    fonts_que(fonts, objectSelector->font_id, fontPos);
    /* fonts_update(fonts, mapeditor->update_elements, &mapeditor->update_elements_count); */

}

char *mapeditor_getObectFromPoint(MapEditor *mapeditor, SDL_Point *p) {
    MAPEDITOR_DESTRUCTOR(mapeditor);
    for (uint16_t i = 0; i < mapeditor->typeAndCrops_count; ++i) {
        SDL_Point *op = &typeAndCrops[i].position;
        SDL_Rect *or = &typeAndCrops[i].crops[0];
        SDL_Rect rect = {op->x, op->y, or->w * mapeditor->grid * mapeditor->objectSelector_scale, or->h * mapeditor->grid * mapeditor->objectSelector_scale};
        if(SDL_PointInRect(p, &rect)) {
            return typeAndCrops[i].type;
        }
    }
    return NULL;
}

static bool _pressedModifier(char *key, const uint8_t *keystate)
{

    if (strstr(key, "LCTRL")) {
        return keystate[SDL_SCANCODE_LCTRL];
    }
    if (strstr(key, "LALT")) {
        return keystate[SDL_SCANCODE_LALT];
    }
    if (strstr(key, "LSHIFT")) {
        return keystate[SDL_SCANCODE_LSHIFT];
    }
    return false;
}

int selectObject(MapEditor *mapeditor, SDL_Point point)
{        
        SDL_Rect *crop = mapeditor_findCrops(mapeditor, mapeditor->objectSelctor_selected);

        Object *o = NULL;
        for (uint16_t i = 0; i < mapeditor->objects_count; ++i) {
            Object *object = &mapeditor->objects[i];
            SDL_Point position = {
                object->position.x,
                object->position.y
            };

            Size size = {
                object->size.w / mapeditor->grid,
                object->size.h / mapeditor->grid 
            };
            SDL_Rect r = {
                position.x,
                position.y,
                size.w,
                size.h
            };
            if (SDL_PointInRect(&point, &r)) {
                /* return object; */
                return i;
            }
        }
    return -1;
}

void placeObject(MapEditor *mapeditor, SDL_Point point)
{
    SDL_Rect *crop = mapeditor_findCrops(mapeditor, mapeditor->objectSelctor_selected);
    Size s1 = {crop->w * mapeditor->grid, crop->h * mapeditor->grid};
    mapeditor_addObject(mapeditor, mapeditor->objectSelctor_selected, &point, &s1, OBJECT_F_VISIBLE);
}

static void _update_events(MapEditor *mapeditor)
{
    static SDL_Point mouse_start = {.x = 0, .y = 0};
    static SDL_Point map_start = {.x = 0, .y = 0};
    bool mouse_button_left = false;
    MAPEDITOR_DESTRUCTOR(mapeditor);
    while (SDL_PollEvent(event)) {
        int key = event->key.keysym.sym;
        const uint8_t *keystate = SDL_GetKeyboardState(NULL);
        /* mouse_get(mouse, mapeditor->scale, mapeditor->grid); */
        mouse_get(mouse, mapeditor->size, 1, mapeditor->grid);

        bool inObjectSelect = \
            mouse->relative_position.x > (sdlData->size.w - objectSelector->width);

        /* char *type = mapeditor_getObectFromPoint(mapeditor, &mouse->relative_position); */

        if (mouse->button != SDL_BUTTON_MIDDLE) {
            mouse_start = mouse->absolute_position;
            map_start = mapeditor->absolute_position;
        }

        switch(mouse->button) {
            case SDL_BUTTON_MIDDLE: {
                SDL_Point current = mouse->absolute_position;
                SDL_Point offset = {
                    .x = mouse_start.x - current.x,
                    .y = mouse_start.y - current.y,
                };
                mapeditor->absolute_position.x = -(offset.x / mapeditor->zoom) + map_start.x;
                mapeditor->absolute_position.y = -(offset.y / mapeditor->zoom) + map_start.y;
                break;
            }
            case SDL_BUTTON_LEFT: {
                mouse_button_left = true;
                /* if (type && inObjectSelect) { */
                /*     if (mapeditor->toggleFlags & TOGGLE_OBJECTSELECT) { */
                /*         mapeditor->objectSelctor_selected = type; */
                /*     } */ 
                /* } else { */
                /*     // TODO */
                /*     // Make sure this only triggers once, create trigger.h to do */
                /*     // that */
                /*     SDL_Point p1 = { */
                /*         mouse->grid_position.x - mapeditor->grid_position.x, */
                /*         mouse->grid_position.y - mapeditor->grid_position.y */
                /*     }; */
                /*     SDL_Rect *crop = mapeditor_findCrops(mapeditor, mapeditor->objectSelctor_selected); */
                /*     printf("%d, %d, %d, %d\n", crop->x, crop->y, crop->w, crop->h); */
                /*     Size s1 = {crop->w, crop->h}; */
                /*     mapeditor_addObject(mapeditor,
                 *     mapeditor->objectSelctor_selected, &p1, &s1); */
                /* } */
                break;
            }
        }

        ControlsKV *control = NULL;
        bool hasCombo = false;
        char keyComb[2][50];

        for (int i = 0; i < mapeditor->controls_length; ++i) {
            ControlsKV *local_control = &mapeditor->controls[i];
            hasCombo = _parse_combo(local_control->key, keyComb);
            bool combo = _pressedModifier(keyComb[0], keystate);
            if (!strcmp(keyComb[1], SDL_GetKeyName(key)) && hasCombo && combo) {
                control = local_control;
                break;
            }
        }

        if (!control) {
            for (int i = 0; i < mapeditor->controls_length; ++i) {
                ControlsKV *local_control = &mapeditor->controls[i];
                hasCombo = _parse_combo(local_control->key, keyComb);
                if (!strcmp(keyComb[1], SDL_GetKeyName(key)) && !hasCombo) {
                    control = local_control;
                    break;
                }
            }
        }

        switch (event->type) {
        case SDL_KEYDOWN: 
        {

            if (!control) return;
            switch (mapeditor->mode) {
               case MODE_PLACE: 
               {
                     if (!strcmp(control->alias, "move_right")) {
                        mapeditor->absolute_position.x -= 20;
                     } else if (!strcmp(control->alias, "move_left")) {
                         mapeditor->absolute_position.x += 20;
                     } else if (!strcmp(control->alias, "move_up")) {
                         mapeditor->absolute_position.y += 20;
                     } else if (!strcmp(control->alias, "move_down")) {
                         mapeditor->absolute_position.y -= 20;
                     }
                   break;
               } 
               case MODE_MOVE: 
               {
                    if (!mapeditor->object_seleted) break;
                    Object *object = &mapeditor->objects[mapeditor->selected_object_id];
                    if (mapeditor->object_seleted) {
                        if (!strcmp(control->alias, "move_left")) {
                             object->position.x -= mapeditor->grid;
                        } else if (!strcmp(control->alias, "move_up")) {
                             object->position.y -= mapeditor->grid;
                        } else if (!strcmp(control->alias, "move_down")) {
                             object->position.y += mapeditor->grid;
                        } else if (!strcmp(control->alias, "move_right")) {
                             object->position.x += mapeditor->grid;
                        }
                    }
                   
                   break;
               }
               case MODE_SELECT: 
               {
                    if (mapeditor->object_seleted ) {
                        /* Object *object = &mapeditor->objects[mapeditor->selected_object_id]; */
                        if (!strcmp(control->alias, "delete") && mapeditor->objects_count > 0) {
                            printf("delete!\n");
                            mapeditor->objects_count -= 1;
                            for (uint16_t i = mapeditor->selected_object_id; i < mapeditor->objects_count; ++i) {
                                mapeditor->objects[i] = mapeditor->objects[i + 1];

                                
                            }
                            MEMRES(mapeditor->objects, sizeof(Object) * mapeditor->objects_count);
                            mapeditor->object_seleted = false;
                        } 
                    }
                   break;
               }
            }
                if (mapeditor->toggleFlags & TOGGLE_OBJECTSELECT && inObjectSelect) {
                    if (!strcmp(control->alias, "zoom_in")) {
                        mapeditor->objectSelector_scale += mapeditor->zoom_size;
                    } else if (!strcmp(control->alias, "zoom_out")) {
                        mapeditor->objectSelector_scale -= mapeditor->zoom_size;
                    }
                } else {
                    if (!strcmp(control->alias, "zoom_in")) {
                        mapeditor->zoom += mapeditor->zoom_size;
                    } else if (!strcmp(control->alias, "zoom_out")) {
                        mapeditor->zoom -= mapeditor->zoom_size;
                    }
                }

            if (!strcmp(control->alias, "save")) {
                char *save_path = RESPATH "/data";
                bool has_save_path = access(save_path, F_OK) == 0;
                if (!has_save_path) {
                    MKDIR(save_path);
                }

                char world_path[255];
                sprintf(world_path, RESPATH "/data/%s/", mapeditor->world_name);
                world_init(&mapeditor->world, world_path, mapeditor->spritesheet_id);
                /* free(mapeditor->world.objects); */
                mapeditor->world.objects = NULL;
                mapeditor->world.map.objects_count = 0;

                mapeditor->world.map.grid = mapeditor->grid;

                /* mapeditor->world.map.objects_count = mapeditor->objects_count - mapeditor->typeAndCrops_count; */
                printf("world count: %d\n editor count: %d\n", mapeditor->world.map.objects_count, mapeditor->objects_count);
                mapeditor->world.map.size.w = mapeditor->size.w;
                mapeditor->world.map.size.h = mapeditor->size.h;
                memcpy(mapeditor->world.map.background_path, mapeditor->background_image_file, 50);
                for (uint16_t i = mapeditor->typeAndCrops_count; i < mapeditor->objects_count; ++i) {
                    Object *object =  &mapeditor->objects[i];
                    printf("editor save type: %s\n", object->type);
                    world_addObject(&mapeditor->world, &mapeditor->objects[i]);
                }
                world_save(&mapeditor->world, mapeditor->map_id);
                printf("successfully saved the map\n");
            } else if (!strcmp(control->alias, "add_object")) {
                SDL_Point p1 = {
                    mouse->relative_position.x - mapeditor->grid_position.x,
                    mouse->relative_position.y - mapeditor->grid_position.y
                };
                SDL_Rect *crop = mapeditor_findCrops(mapeditor, "floor");
                Size s1 = {crop->w * mapeditor->grid, crop->h * mapeditor->grid};
                /* mapeditor_addObject(mapeditor, "floor", &p1, &s1, OBJECT_F_VISIBLE); */
                /* mapeditor_addObject(mapeditor, "floor", &p1, &s1, OBJECT_F_VISIBLE); */
                
            } else if (!strcmp(control->alias, "position_reset")) {
                mapeditor->absolute_position.x  = 0;
                mapeditor->absolute_position.y  = 0;
                mapeditor->zoom = mapeditor->zoom_default;
            } else if (!strcmp(control->alias, 
                        "toggle_object_select")) {
                if (mapeditor->toggleFlags & TOGGLE_OBJECTSELECT) {
                    mapeditor->toggleFlags &= ~TOGGLE_OBJECTSELECT;
                } else {
                    mapeditor->toggleFlags |= TOGGLE_OBJECTSELECT;
                }
            } else if (!strcmp(control->alias, "toggle_info")) {
                if (mapeditor->toggleFlags & TOGGLE_INFO) {
                    mapeditor->toggleFlags &= ~TOGGLE_INFO;
                } else {
                    mapeditor->toggleFlags |= TOGGLE_INFO;
                }
            } else if (!strcmp(control->alias, "next")) {
                if (mapeditor->toggleFlags & TOGGLE_INFO) {
                        worldInfo->position = 
                            (worldInfo->position + 1) % WORLDINFO_COUNT;
                }
            } else if (!strcmp(control->alias, "previous")) {
                if (mapeditor->toggleFlags & TOGGLE_INFO) {
                    if (worldInfo->position - 1 > 0) {
                        worldInfo->position--;
                    } else if (worldInfo->position - 1 < 0) {
                        worldInfo->position = WORLDINFO_COUNT - 1;
                    }
                }
            } else if (!strcmp(control->alias, "toggle_fullscreen")) {
                if (mapeditor->toggleFlags & TOGGLE_OBJECTSELECT) {
                    objectSelector->fullscreen_toggle = !objectSelector->fullscreen_toggle;
                    objectSelector->offset = 0;
                }
            } else if (!strcmp(control->alias, "mode_select")) {
                mapeditor->mode = MODE_SELECT;
            } else if (!strcmp(control->alias, "mode_place")) {
                mapeditor->mode = MODE_PLACE;
            } else if (!strcmp(control->alias, "mode_move")) {
                mapeditor->mode = MODE_MOVE;
            }
            break;
        } // case
        case SDL_KEYUP:
        {
            break;
        } // case
        case SDL_QUIT:
        {
            mapeditor->quit = true;
            break;
        } // case
        case SDL_MOUSEWHEEL:
        {
            if ((mapeditor->toggleFlags & TOGGLE_OBJECTSELECT) &&
                    inObjectSelect) {
                if (event->wheel.y > 0) { // scroll up
                    objectSelector->offset += 10;
                }
                if (event->wheel.y < 0) { // scroll down
                    objectSelector->offset -= 10;
                }
            } else {
                if (event->wheel.y > 0 && keystate[SDL_SCANCODE_LCTRL]) { // scroll up
                    SDL_Point p1 = {
                        (mouse->grid_position.x / mapeditor->zoom) - mapeditor->grid_position.x,
                        (mouse->grid_position.y / mapeditor->zoom) - mapeditor->grid_position.y
                    };
                    mapeditor->zoom += mapeditor->zoom_size;
                    SDL_Point p2 = {
                        (mouse->grid_position.x / mapeditor->zoom) - mapeditor->grid_position.x,
                        (mouse->grid_position.y / mapeditor->zoom) - mapeditor->grid_position.y
                    };
                    SDL_Point p3 = {
                        .x = p2.x - p1.x,
                        .y = p2.y - p1.y,
                    };
                    mapeditor->absolute_position.x += p3.x * mapeditor->grid;
                    mapeditor->absolute_position.y += p3.y * mapeditor->grid;

                }
                if (event->wheel.y < 0 && keystate[SDL_SCANCODE_LCTRL]) { // scroll down
                    if (mapeditor->zoom <= mapeditor->zoom_size) break;
                    SDL_Point p1 = {
                        (mouse->grid_position.x / mapeditor->zoom) - mapeditor->grid_position.x,
                        (mouse->grid_position.y / mapeditor->zoom) - mapeditor->grid_position.y
                    };
                    mapeditor->zoom -= mapeditor->zoom_size;
                    SDL_Point p2 = {
                        (mouse->grid_position.x / mapeditor->zoom) - mapeditor->grid_position.x,
                        (mouse->grid_position.y / mapeditor->zoom) - mapeditor->grid_position.y
                    };
                    SDL_Point p3 = {
                        .x = p2.x - p1.x,
                        .y = p2.y - p1.y,
                    };
                    mapeditor->absolute_position.x += p3.x * mapeditor->grid;
                    mapeditor->absolute_position.y += p3.y * mapeditor->grid;
                    /* mapeditor->absolute_position.x += mouse->g_x; */
                    /* mapeditor->absolute_position.y += mouse->g_y; */
                    /* mapeditor->absolute_position.x += mapeditor->zoom * mapeditor->grid; */
                    /* mapeditor->absolute_position.y += mapeditor->zoom * mapeditor->grid; */
                }

            }
            break;
        } // case
        } // switch
    }
    bool inObjectSelect = \
        mouse->relative_position.x > (sdlData->size.w - objectSelector->width);
    char *type = mapeditor_getObectFromPoint(mapeditor, &mouse->relative_position);

     SDL_Point p = {
            (mouse->grid_position.x / mapeditor->zoom) - mapeditor->grid_position.x,
            (mouse->grid_position.y / mapeditor->zoom) - mapeditor->grid_position.y
        };
    SDL_Point qp = {
        .x = quantize(p.x, mapeditor->grid ),
        .y = quantize(p.y, mapeditor->grid),
    };
    if (mouse_button_left) {
        if (type && inObjectSelect &&
                (mapeditor->toggleFlags & TOGGLE_OBJECTSELECT)) {
            if (mapeditor->toggleFlags & TOGGLE_OBJECTSELECT) {
                mapeditor->objectSelctor_selected = type;
            } 
        } else {
            // check if your on object selector or object placer
            switch (mapeditor->mode) {
                case MODE_PLACE: 
                {
                   mapeditor->object_seleted = false;
                    placeObject(mapeditor, qp);
                    break;
                }
                case MODE_SELECT: 
                {
                   int id =  selectObject(mapeditor, qp);
                   if (id == -1) break;
                   mapeditor->object_seleted = true;
                   mapeditor->selected_object_id = id;
                   Object *object = &mapeditor->objects[id];
                    if (object) {
                        printf("%s\n", object->type);
                    }
                    break;
                }
                case MODE_MOVE: 
                {
                    if (!mapeditor->object_seleted) break;
                    Object *object = &mapeditor->objects[mapeditor->selected_object_id];
                    if (object) {
                        object->position.x = qp.x;
                        object->position.y = qp.y;
                    }
                    break;
                }
            }
        }

    }
    /* if (mapeditor->object_seleted) { */
    /*     uint16_t world_id = mapeditor->selected_object_id - mapeditor->typeAndCrops_count; */
    /*     Object *object = &mapeditor->objects[mapeditor->selected_object_id]; */
    /*     world_changeObject(&mapeditor->world, world_id, object); */
    /* } */
}

void mapeditor_update(MapEditor *mapeditor)
{
    MAPEDITOR_DESTRUCTOR(mapeditor);
    int spritesheet_id = mapeditor->spritesheet_id;
    int background_id = mapeditor->background_id;

    SDL_Rect r = {
        mapeditor->absolute_position.x * mapeditor->zoom,
        mapeditor->absolute_position.y * mapeditor->zoom,
        mapeditor->size.w * mapeditor->zoom,
        mapeditor->size.h * mapeditor->zoom,
        /* mapeditor->images.screenElements[mapeditor->background_id].position.w * mapeditor->zoom, */
        /* mapeditor->images.screenElements[mapeditor->background_id].position.h * mapeditor->zoom */
    };

    /* images_updateQue(images, mapeditor->background_id, NULL); */
    images_que(images, mapeditor->background_id, &r);
    for (uint16_t i = mapeditor->typeAndCrops_count; i < mapeditor->objects_count; ++i) {
        Object *object = &mapeditor->objects[i];


        int grid_x = (object->position.x) * mapeditor->grid;
        int grid_y = (object->position.y) * mapeditor->grid;
        int offset_x = (grid_x + mapeditor->absolute_position.x);
        int offset_y = (grid_y + mapeditor->absolute_position.y);

        SDL_Point position = {
            (offset_x * mapeditor->zoom),
            (offset_y * mapeditor->zoom),
        };

        Size size = {
           object->size.w * mapeditor->zoom,
            object->size.h * mapeditor->zoom
        };

        sprites_updateCrop2(
                sprites, 
                sdlData->renderer,
                position,
                size,
                0,
                spritesheet_id,
                object->crop_id
        );
    }
    if (mapeditor->toggleFlags & TOGGLE_OBJECTSELECT) {
        mapeditor_objectSelectorUpdate(mapeditor);
    }
    if (mapeditor->toggleFlags & TOGGLE_INFO) {
        uint8_t i = worldInfo->position;
        SDL_Rect *font_rect = fonts_getRect(fonts, worldInfo->ids[i]);

        int x = (sdlData->size.w -  font_rect->w) / 2;


        SDL_Point fontPos = {
             x,
             sdlData->size.h - font_rect->h
        };

        fonts_que(fonts, worldInfo->ids[i], fontPos);

    }

    SDL_SetRenderDrawBlendMode(sdlData->renderer, SDL_BLENDMODE_NONE);
    images_updateQue(images, mapeditor->update_elements, &mapeditor->update_elements_count);
    sprites_updateQue(sprites, mapeditor->update_elements, &mapeditor->update_elements_count);
    fonts_updateQue(fonts, mapeditor->update_elements, &mapeditor->update_elements_count);
    SDL_SetRenderDrawBlendMode(sdlData->renderer, SDL_BLENDMODE_BLEND);
    rects_updateQue(mapeditor->update_elements, &mapeditor->update_elements_count);
    lines_updateQue(lines, mapeditor->update_elements, &mapeditor->update_elements_count);
    /* lines_update(lines, sdlData->renderer); */

    mapeditor->grid_position.x = floor((float)mapeditor->absolute_position.x /
                                            (float)mapeditor->grid);

    mapeditor->grid_position.y = floor((float)mapeditor->absolute_position.y /
                                            (float)mapeditor->grid);

    SDL_Rect rect = {
        .x = 0,
        .y = 0,
        .w = mapeditor->size.w,
        .h = mapeditor->size.h,
    };
    screenElements_update(mapeditor->update_elements, mapeditor->update_elements_count, sdlData->renderer, &rect);
    mapeditor->update_elements_count = 0;


    _update_events(mapeditor);
    SDL_RenderPresent(sdlData->renderer);
    fonts_reset(fonts);
    sprites_reset(sprites);
    images_reset(images);
    rects_reset();
    lines_reset(lines);
    SDL_Delay(10);
}

#endif // N2DE_MAPEDITOR_H_
