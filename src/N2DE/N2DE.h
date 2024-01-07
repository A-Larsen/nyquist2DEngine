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
#ifndef N2DE_H_
#define N2DE_H_

#include "rects.h"
#define _GNU_SOURCE

#include <stdint.h>
#include <stdint.h>
#include <stdbool.h>

#include "config.h"
#include "memory.h"
#include "error.h"
#include "world.h"
#include "declare.h"
#include "lua/begin.h"
#include "lua/globals.h"
#include "lua/text.h"
#include "lua/images.h"
/* #include "lua/database.h" */
/* #include "lua/sprites.h" */
#include "lua/loops.h"
#include "lua/world.h"
#include "lua/players.h"
#include "lua/map.h"
#include "lua/rects.h"

#define SDL_main main
#define VERSION (uint32_t)0x00000001
#define VERSION_RELEASE  (VERSION >> 24)
#define VERSION_BETA     (VERSION >> 16 & 0xFF)
#define VERSION_REVISION (VERSION >> 8 & 0xFF)
#define VERSION_BUILD    (VERSION & 0xFF)
#define OPTION_CAPTURE_MEMORY 1 << 0
#define OPTION_VERBOSE        1 << 1


uint64_t option_flags = 0;

void luaF_newFuncsTable(lua_State *lua_state, char *name, 
                       const struct luaL_Reg *funcs) 
{
    lua_pushstring(lua_state, name); \
    lua_newtable(lua_state); \
    for (const struct luaL_Reg *reg = funcs; reg->name; ++reg) { \
        lua_pushstring(lua_state, reg->name); \
        lua_pushcfunction(lua_state, reg->func); \
        lua_settable(lua_state, -3); \
    }
}

void luaF_setFuncsTable(lua_State *lua_state, const struct luaL_Reg *funcs)
{
    for (const struct luaL_Reg *reg = funcs; reg->name; ++reg) { \
        lua_pushstring(lua_state, reg->name); \
        lua_pushcfunction(lua_state, reg->func); \
        lua_settable(lua_state, -3); \
    }
}

void engine_init(Nyquist2DEngine *engine, const char *lua_path)
{
    bool hasResFolder = access(RESPATH, F_OK) == 0;
    engine->running = false;

    if (!hasResFolder) {
        MKDIR(RESPATH);
    }

    bool hasDataFolder = access(RESPATH "/data/", F_OK) == 0;

    if (!hasDataFolder) {
        MKDIR(RESPATH "/data/");
    }

    bool hasFontsFolder = access(RESPATH "/fonts/", F_OK) == 0;

    if (!hasFontsFolder) {
        MKDIR(RESPATH "/fonts/");
    }

    memset(engine, 0, sizeof(Nyquist2DEngine));
    if (TTF_Init() == -1) {
        N2DE_ERROR(TTF_GetError());
        exit(1);
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        N2DE_ERROR(IMG_GetError());
        exit(1);
    }
    /* circles_init(&engine->circles); */
    rects_init();
    fonts_init(&engine->fonts);
    fonts_init(&engine->format_strings);
    images_init(&engine->images);
    loops_init(&engine->loops);


    engine->lua_state = luaL_newstate();
    engine->initialized = false;
    engine->hasFrameCap = true;
    luaL_openlibs(engine->lua_state);

    memcpy(engine->lua_path, lua_path, strlen(lua_path) + 1);

#ifdef PRODUCTION
    lua_pushboolean(engine->lua_state, true);
    lua_setglobal(engine->lua_state, "PRODUCTION");
#endif

    lua_pushnumber(engine->lua_state, SQLITE_INTEGER);
    lua_setglobal(engine->lua_state, "DB_INTEGER");

    lua_pushnumber(engine->lua_state, SQLITE_TEXT);
    lua_setglobal(engine->lua_state, "DB_TEXT");

    lua_pushnumber(engine->lua_state, SQLITE_FLOAT);
    lua_setglobal(engine->lua_state, "DB_FLOAT");

    lua_pushnumber(engine->lua_state, SQLITE_BLOB);
    lua_setglobal(engine->lua_state, "DB_BLOB");

    lua_pushnumber(engine->lua_state, SQLITE_FOREIGN);
    lua_setglobal(engine->lua_state, "DB_FOREIGN");

    lua_pushnumber(engine->lua_state, SQLITE_INTEGER_UNIQUE);
    lua_setglobal(engine->lua_state, "DB_INTEGER_UNIQUE");

    lua_pushnumber(engine->lua_state, SQLITE_TEXT_UNIQUE);
    lua_setglobal(engine->lua_state, "DB_TEXT_UNIQUE");

    /* lua_pushnil(engine->lua_state); */
    /* lua_setglobal(engine->lua_state, "key"); */


    /* lui */
    lua_newtable(engine->lua_state);

    luaF_setFuncsTable(engine->lua_state, luaFunctions_global);

    lua_pushstring(engine->lua_state, "grid");
    lua_pushnumber(engine->lua_state, 0);
    lua_settable(engine->lua_state, -3); // nyq

    lua_pushstring(engine->lua_state, "keydown");
    lua_pushboolean(engine->lua_state, false);
    lua_settable(engine->lua_state, -3); // nyq

    lua_pushstring(engine->lua_state, "_cdata");
    lua_pushlightuserdata(engine->lua_state, (void *)engine);
    lua_settable(engine->lua_state, -3); // nyq
                                         //
    lua_pushstring(engine->lua_state, "keypress");
    lua_newtable(engine->lua_state);
    lua_settable(engine->lua_state, -3);

    lua_pushstring(engine->lua_state, "quit");
    lua_pushboolean(engine->lua_state, false);
    lua_settable(engine->lua_state, -3); // nyq

    luaF_newFuncsTable(engine->lua_state, "text", luaFunctions_text);
    lua_settable(engine->lua_state, -3); // nyq

    luaF_newFuncsTable(engine->lua_state, "images", luaFunctions_images);
    lua_settable(engine->lua_state, -3); // nyq

    /* luaF_newFuncsTable(engine->lua_state, "database", luaFunctions_database); */
    /* lua_settable(engine->lua_state, -3); // nyq */
                                         //
    /* luaF_newFuncsTable(engine->lua_state, "sprites", luaFunctions_sprites); */
    /* lua_settable(engine->lua_state, -3); // nyq */

    luaF_newFuncsTable(engine->lua_state, "loops", luaFunctions_loops);
    lua_settable(engine->lua_state, -3); // nyq


    luaF_newFuncsTable(engine->lua_state, "world", luaFunctions_world);
    lua_settable(engine->lua_state, -3); // nyq
                                         //
    luaF_newFuncsTable(engine->lua_state, "players", luaFunctions_players);
    lua_settable(engine->lua_state, -3); // nyq
                                         //
    luaF_newFuncsTable(engine->lua_state, "map", luaFunctions_map);
    lua_settable(engine->lua_state, -3); // nyq
                                         //
    luaF_newFuncsTable(engine->lua_state, "rects", luaFunctions_rects);
    lua_settable(engine->lua_state, -3); // nyq

    /* luaF_newFuncsTable(engine->lua_state, "formats", luaFunctions_formats); */
    /* lua_settable(engine->lua_state, -3); // nyq */


    lua_setglobal(engine->lua_state, "nyq");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO)) {
        N2DE_ERROR("could not initialize SDL\n%s", SDL_GetError());
        exit(1);
    }
}


void engine_run(Nyquist2DEngine *engine)
{

    /* for(int i = 0; i < 4; ++i){ */
        
    /*     for (int j = 0; j < engine->players.playerinfo[i].controls_length; ++j) { */
    /*             engine->controls[j].isPressed = false; */
    /*             engine->controls[j].triggered = false; */
    /*     } */
    /* } */

    if (luaL_dofile(engine->lua_state, engine->lua_path) != LUA_OK) {
        N2DE_ERROR("Error reading script\n %s\n",
                    lua_tostring(engine->lua_state, -1));
    }
}

void engine_destroy(Nyquist2DEngine *engine)
{
    for(int i = 0; i < engine->inputs.count; ++i){
        if (engine->inputs.inputInfo[i].controller_id > -1) {
            SDL_GameControllerClose(engine->inputs.inputInfo[i].controller);
        }
    }
    SDL_DestroyRenderer(engine->renderer);
    fonts_quit(&engine->fonts);
    formats_quit();
    fonts_quit(&engine->format_strings);
    images_quit(&engine->images);
    database_quit(&engine->database);
    SDL_DestroyWindow(engine->window);
    SDL_Quit();
    world_free(&engine->world);
    /* lua_freeSchema(engine->lua_state); */
    lua_close(engine->lua_state);
    /* sprites_free(&engine->sprites); */
    loops_free(&engine->loops);
    /* audio_quit(&engine->audio); */
    /* circles_free(&engine->circles); */
    rects_free();
    lines_free(&engine->lines);
    TTF_Quit();
    IMG_Quit();
    printf("end\n");
}

int engine_handleOptions(int args, char **argv)
{
    bool noFile = false;
    if (args == 1) {
        if (access("./init.lua", F_OK) == 0) {
            noFile = true;
        } else {
            N2DE_ERROR("cound not find lua script");
        }
    }

    char option[50];
    char argument[50];

    char **start = argv;

    for (argv++;*argv && strstr(*argv, "--"); argv++) {

        if (strcmp(*argv, "--wipe") == 0) {
            memcpy(option, *argv, strlen(*argv));
            argv++;
            if (*argv) {
                memcpy(argument, *argv, strlen(*argv));
                if (strcmp(*argv, "data") == 0) {
                    printf("removing data\n");
                    removeDir(RESPATH "/data");
                } else {
                    N2DE_ERROR("%s:\n\tinvalid argument '%s'",
                            option, argument);
                    exit(1);
                }
            } else {
                N2DE_ERROR("%s:\n\tno argument provided",
                        option);
                exit(1);
            };
        }
        if (strcmp(*argv, "--version") == 0) {
            printf("%s %d.%d.%d.%d\n", PROGNAME,
                    VERSION_RELEASE, VERSION_BETA, VERSION_REVISION,
                    VERSION_BUILD);
            exit(0);
        }
    }

    if (noFile) {
        return -1;
    } else {
        return argv - start;
    }
}

#endif // N2DE_H_
