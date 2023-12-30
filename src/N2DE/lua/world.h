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

bool WORLD_UPDATE = false;
float WORLD_SCALE = 1;

int luaWorld_init(lua_State *L)
{

    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    char *spritesheet = (char *)luaL_checkstring(L, 1);
    char *world = (char *)luaL_checkstring(L, 2);
    char spritesheet_path[255];
    sprintf(spritesheet_path, RESPATH "/images/%s", spritesheet);
    Size size = {.w = 1, .h = 1};
    /* uint8_t id = sprites_new(&engine->sprites, engine->renderer, spritesheet_path, */
    /*                           NULL, 0); */
    uint8_t id = sprites_new(&engine->sprites, engine->renderer, spritesheet_path);
    char world_path[255];
    sprintf(world_path, RESPATH "/data/%s/", world);

    world_init(&engine->world, world_path, id);
    return 0;
}

int luaWorld_read(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int id = (int)luaL_checknumber(L, 1);
    int zIndex = (int)luaL_checknumber(L, 2);
    uint16_t group_id = world_read(&engine->world, id, engine->renderer, &engine->images, &engine->sprites, zIndex);
    return group_id;
}

int luaWorld_print(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    world_print(&engine->world);
    return 0;
}

int luaWorld_update(lua_State *L)
{
    float scale = (float)luaL_checknumber(L, 1);
    WORLD_UPDATE = true;
    WORLD_SCALE = scale;

    /* world_update(&engine->world, engine->renderer, &engine->sprites); */
    return 0;
}

int luaWorld_addCollisionCheck(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    SDL_Point p;
    Size s;
    lua_topoint(L, 1, &p);
    lua_tosize(L, 2, &s);
    SDL_Rect r = {
        p.x, p.y, s.w, s.h
    };
    uint16_t i = world_addCollisionCheck(&engine->world, &r);
    lua_pushnumber(L, i);
    return 1;
}

int luaWorld_checkCollision(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    uint16_t id = (uint16_t)luaL_checknumber(L, 1);
    Collider *collider = NULL;
    collider = world_checkCollision(&engine->world, id);

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
    uint8_t grid = engine->world.map.grid;
    lua_pushnumber(L, grid);
    return 1;
}

int luaWorld_free(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    world_free(&engine->world);
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
    {"free", luaWorld_free},
    /* {"isCollidable", luaWorld_isCollidable}, */
    {NULL, NULL}
};

#endif // N2DE_LUA_WORLD_H_
