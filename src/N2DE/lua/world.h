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
#ifndef N2DE_LUA_WORLD_H_
#define N2DE_LUA_WORLD_H_

#include <stdint.h>
#include <stdio.h>

#include "begin.h"

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

int luaWorld_init(lua_State *L)
{

    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    int num = (int)luaL_checknumber(L, 1);
    char *spritesheet = (char *)luaL_checkstring(L, 2);
    char *world = (char *)luaL_checkstring(L, 3);

    char spritesheet_path[255];
    sprintf(spritesheet_path, RESPATH "/images/%s", spritesheet);
    Size size = {.w = 1, .h = 1};
    /* uint8_t id = sprites_new(&engine->sprites, engine->renderer, spritesheet_path, */
    /*                           NULL, 0); */
    uint8_t id = sprites_new(&engine->sprites, engine->renderer, spritesheet_path);
    char world_path[255];
    sprintf(world_path, RESPATH "/data/%s/", world);

    world_init(&engine->worlds[num], world_path, id);
    /* engine->worlds[num].int = true; */
    return 0;
}

int luaWorld_addObject(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    int num = (int)luaL_checknumber(L, 1);
    World *world = &engine->worlds[num];
    Object object;
    memset(&object, 0, sizeof(Object));

    if (lua_istable(L, 2) == NULL) return 0;

    lua_getfield(L, 2, "type");
    const char *type = luaL_checkstring(L, -1);
    memset(object.type, 0, 25);
    memcpy(object.type, type, strlen(type));
    lua_pop(L, 1); // position

    lua_getfield(L, 2, "position");
    lua_topoint(L, -1, &object.position);
    lua_pop(L, 1); // position

    lua_getfield(L, 2, "size");
    lua_tosize(L, -1, &object.size);
    lua_pop(L, 1); // size

    lua_getfield(L, 2, "hitbox");
    SDL_Rect r1;
    lua_torect(L, -1, &r1);
   SDL_Rect r2 = {
        .x = r1.x / world->map.grid,
        .y = r1.y / world->map.grid,
        .w = r1.w / world->map.grid,
        .h = r1.h / world->map.grid,
    };
    memcpy(&object.hitbox, &r2, sizeof(SDL_Rect));
    lua_pop(L, 1); // hitbox


    lua_getfield(L, 2, "is_collidable");
    object.is_collidable = lua_toboolean(L, -1);
    lua_pop(L, 1); // is_collidable

    lua_getfield(L, 2, "is_visable");
    object.is_visable = lua_toboolean(L, -1);
    lua_pop(L, 1); // is_visable

    lua_getfield(L, 2, "crops");

    if (lua_istable(L, -1)) {
        printf("is a table\n");
        int i = 0;
        lua_pushnil(L);
        while(lua_next(L, -2)) {
            lua_torect(L, -1, &object.crops[i]);
            lua_pop(L, 1);
            i++;
        }
    }
    lua_pop(L, 1); // crops

    uint8_t zIndex = 0;
    int i = world_addObject(world, &object);

        world->objects[i].crop_id = \ 
            sprites_addCrops(&engine->sprites, engine->renderer, world->spritesheet_id, world->objects[i].type,
                             world->objects[i].crops, CROP_VARIATION_COUNT, zIndex);
        SDL_Rect back = {
            object.crops[0].x,
            object.crops[0].y,
            object.crops[0].w,
            object.crops[0].h,
        };
        SDL_Rect front = {
            object.crops[0].x,
            object.crops[0].y,
            object.crops[0].w,
            object.hitbox.y,
        };
        if (!SDL_RectEquals(&front, &back) && object.is_visable) {
            uint16_t j = world->front_objects_count++;
            MEMRES(world->front_objects, sizeof(FrontObject) * world->front_objects_count);
            world->front_objects[j].crop_id = sprites_addCrops(&engine->sprites, engine->renderer, 
                    world->spritesheet_id, object.type, &front,
                    1, zIndex + 2);
            world->front_objects[j].object_id = i;
            memcpy(&world->front_objects[j].rect, &front, sizeof(SDL_Rect));
        }

    return 0;
}

int luaWorld_getObjects(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    int num = (int)luaL_checknumber(L, 1);
    World *world = &engine->worlds[num];

    lua_newtable(L);
    for (int i = 0; i < world->map.objects_count; ++i) {
        Object *object = &engine->worlds[num].objects[i];

        lua_pushnumber(L, i + 1);

        lua_newtable(L);

        lua_pushstring(L, "type");
        lua_pushstring(L, object->type);
        lua_settable(L, -3);

        lua_pushstring(L, "is_visable");
        lua_pushboolean(L, object->is_visable);
        lua_settable(L, -3);

        lua_pushstring(L, "is_collidable");
        lua_pushboolean(L, object->is_collidable);
        lua_settable(L, -3);

        lua_pushstring(L, "is_visable");
        lua_pushboolean(L, object->is_visable);
        lua_settable(L, -3);

        lua_pushstring(L, "was_collected");
        lua_pushboolean(L, object->wasCollected);
        lua_settable(L, -3);

        lua_pushstring(L, "position");
        lua_newtable(L);
        lua_pushstring(L, "x");
        lua_pushnumber(L, object->position.x);
        lua_settable(L, -3);
        lua_pushstring(L, "y");
        lua_pushnumber(L, object->position.y);
        lua_settable(L, -3);
        lua_settable(L, -3);

        lua_pushstring(L, "size");
        lua_newtable(L);
        lua_pushstring(L, "w");
        lua_pushnumber(L, object->size.w);
        lua_settable(L, -3);
        lua_pushstring(L, "h");
        lua_pushnumber(L, object->size.h);
        lua_settable(L, -3);
        lua_settable(L, -3);


        lua_settable(L, -3);
    }

    return 1;
}

int luaWorld_read(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int num = (int)luaL_checknumber(L, 1);
    int id = (int)luaL_checknumber(L, 2);
    int zIndex = (int)luaL_checknumber(L, 3);
    uint8_t group_id = world_read(&engine->worlds[num], id, engine->renderer, &engine->images, &engine->sprites, zIndex);
    return group_id;
}

int luaWorld_print(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    int num = (int)luaL_checknumber(L, 1);
    world_print(&engine->worlds[num]);
    return 0;
}

int luaWorld_update(lua_State *L)
{
    int num = (int)luaL_checknumber(L, 1);
    float scale = (float)luaL_checknumber(L, 2);
    WORLD_UPDATE[num] = true;
    WORLD_SCALE[num] = scale;

    /* world_update(&engine->world, engine->renderer, &engine->sprites); */
    return 0;
}

int luaWorld_addCollisionCheck(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int num = (int)luaL_checknumber(L, 1);
    SDL_Point p;
    Size s;
    lua_topoint(L, 2, &p);
    lua_tosize(L, 3, &s);
    SDL_Rect r = {
        p.x, p.y, s.w, s.h
    };
    uint16_t i = world_addCollisionCheck(&engine->worlds[num], &r);
    lua_pushnumber(L, i);
    return 1;
}

int luaWorld_checkCollision(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int num = (int)luaL_checknumber(L, 1);
    uint16_t id = (uint16_t)luaL_checknumber(L, 2);
    Collider *collider = NULL;
    collider = world_checkCollision(&engine->worlds[num], id);

    lua_newtable(L);
    if (!collider) {
        lua_pushstring(L, "type");
        lua_pushstring(L, "NONE");
        lua_settable(L, -3);

        lua_pushstring(L, "hit_top");
        lua_pushboolean(L, 0);
        lua_settable(L, -3);

        lua_pushstring(L, "hit_right");
        lua_pushboolean(L, 0);
        lua_settable(L, -3);

        lua_pushstring(L, "hit_bottom");
        lua_pushboolean(L, 0);
        lua_settable(L, -3);

        lua_pushstring(L, "hit_left");
        lua_pushboolean(L, 0);
        lua_settable(L, -3);

        lua_pushstring(L, "is_collidable");
        lua_pushboolean(L, 0);
        lua_settable(L, -3);

        lua_pushstring(L, "is_visable");
        lua_pushboolean(L, 0);
        lua_settable(L, -3);
        return 1;
    }

        lua_pushstring(L, "type");
        lua_pushstring(L, collider->object->type);
        lua_settable(L, -3);

        lua_pushstring(L, "hit_top");
        lua_pushboolean(L, collider->hit_top);
        lua_settable(L, -3);

        lua_pushstring(L, "hit_right");
        lua_pushboolean(L, collider->hit_right);
        lua_settable(L, -3);

        lua_pushstring(L, "hit_bottom");
        lua_pushboolean(L, collider->hit_bottom);
        lua_settable(L, -3);

        lua_pushstring(L, "hit_left");
        lua_pushboolean(L, collider->hit_left);
        lua_settable(L, -3);

        lua_pushstring(L, "is_collidable");
        lua_pushboolean(L, collider->object->is_collidable);
        lua_settable(L, -3);

        lua_pushstring(L, "is_visable");
        lua_pushboolean(L, collider->object->is_visable);
        lua_settable(L, -3);

    return 1;
}

int luaWorld_getGrid(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int num = (int)luaL_checknumber(L, 1);
    uint8_t grid = engine->worlds[num].map.grid;
    lua_pushnumber(L, grid);
    return 1;
}

int luaWorld_free(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int num = (int)luaL_checknumber(L, 1);
    world_free(&engine->worlds[num]);
    return 0;
}

int luaWorld_reset(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    for (int i = 0; i < 4; ++i) {
        engine->worlds[i].exists = false;
    }
    return 0;
}


const struct luaL_Reg luaFunctions_world[] = {
    {"init", luaWorld_init},
    {"read", luaWorld_read},
    {"print", luaWorld_print},
    {"update", luaWorld_update},
    {"addCollisionCheck", luaWorld_addCollisionCheck},
    {"checkCollision", luaWorld_checkCollision},
    {"getGrid", luaWorld_getGrid},
    {"getObjects", luaWorld_getObjects},
    {"reset", luaWorld_reset},
    {"free", luaWorld_free},
    {"addObject", luaWorld_addObject},
    /* {"isCollidable", luaWorld_isCollidable}, */
    {NULL, NULL}
};

#endif // N2DE_LUA_WORLD_H_
