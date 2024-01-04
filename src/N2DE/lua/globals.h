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
#ifndef N2DE_LUA_GLOBALS_H_
#define N2DE_LUA_GLOBALS_H_

#define _GNU_SOURCE

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

#include "begin.h"

static int loop_id;

int luaGlobal_delay(lua_State *L)
{
    int time = (int)luaL_checknumber(L, -1);
    SDL_Delay(time);
    return 0;
}

int luaGlobal_init(lua_State *L)
{
    int arg_count = lua_gettop(L);
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    engine->players.count = 1;
    engine->audio.wavInfo = NULL;


    SDL_Point p;
    Size s;
    int term_font_size = 20;
    int term_height = 200;
    SDL_Color term_fg_color = {255, 255, 255, 255};
    SDL_Color term_bg_color = {0, 0, 0, 200};
    SDL_Color term_error_color = {255, 0, 0, 255};
    SDL_Color term_cmd_color = {255, 255, 0, 255};
    uint16_t term_max_history = 20;

    const char *windowName = luaL_checkstring(L, 1);

    lua_topoint(L, 2, &p);
    lua_tosize(L, 3, &s);

    int scale = 1;
    uint8_t frame_rate = 60;
    bool isFullscreen = false;

    // options
    if (arg_count > 3 && lua_istable(L, 4)) {
        int i = 0;
        lua_pushnil(L);
        while(lua_next(L, 4) != 0) {
            const char *option  = luaL_checkstring(L, -2);

            if (!strcmp(option, "terminal")) {

                lua_pushnil(L);
                while(lua_next(L,-2) != 0) {
                    const char *option  = luaL_checkstring(L, -2);

                    if (!strcmp(option, "font_size")) {
                        int num  = (int)luaL_checknumber(L, -1);
                        term_font_size = num;
                    }

                    if (!strcmp(option, "height")) {
                        int num  = (int)luaL_checknumber(L, -1);
                        term_height = num;
                    }
                    if (!strcmp(option, "fg_color")) {
                        lua_tocolor(L, -1, &term_fg_color);
                    }
                    if (!strcmp(option, "bg_color")) {
                        lua_tocolor(L, -1, &term_bg_color);
                    }
                    if (!strcmp(option, "error_color")) {
                        lua_tocolor(L, -1, &term_error_color);
                    }
                    if (!strcmp(option, "cmd_color")) {
                        lua_tocolor(L, -1, &term_cmd_color);
                    }

                    if (!strcmp(option, "max_history")) {
                        uint16_t num  = (uint16_t)luaL_checknumber(L, -1);
                        term_max_history = num;
                    }

                   lua_pop(L, 1);
                }

            }
            if (!strcmp(option, "scale")) {
                int num  = (int)luaL_checknumber(L, -1);
                scale = num;
            }
            if (!strcmp(option, "fullscreen")) {
                isFullscreen  = (bool)lua_toboolean(L, -1);
            }
            if (!strcmp(option, "frame_rate")) {
                    int num  = (int)luaL_checknumber(L, -1);
                    frame_rate = num;
            }
            if (!strcmp(option, "controls")) {
                if (lua_type(L, -1) == LUA_TTABLE) {
                    lua_pushnil(L);

                    int ki = 0;
                    int gi = 0;
                    uint8_t player_id = 0;
                    while(lua_next(L, -2)) {
                        player_id = luaL_checknumber(L, -2);
                        player_id -= 1;
                        printf("lua player id: %d\n", player_id);
                        lua_pushnil(L);
                        while(lua_next(L, -2)) {
                            /* const char *alias  = luaL_checkstring(L, -2); */
                            /* memcpy(engine->players.playerInfo[player_id].keyboard_controls[i].alias, alias, strlen(alias)); */

                            /* lua_getfield(L, -1, "key"); */
                            /* const char *key  = luaL_checkstring(L, -1); */
                            /* lua_pop(L, 1); */

                            /* memcpy(engine->players.playerInfo[player_id].keyboard_controls[i].key, key, strlen(key)); */

                            /* lua_getfield(L, -1, "trigger"); */
                            /* bool trigger = (bool)lua_toboolean(L, -1); */
                            /* lua_pop(L, 1); */

                            /* engine->players.playerInfo[player_id].keyboard_controls[i].keyTriggered = trigger; */

                            /* lua_getfield(L, -1, "keyrepeat"); */
                            /* bool repeat = (bool)lua_toboolean(L, -1); */
                            /* engine->players.playerInfo[player_id].keyboard_controls[i].repeat = repeat; */
                            /* lua_pop(L, 1); */

                            /* engine->players.playerInfo[player_id].keyboard_controls[i].isPressed = false; */
                            /* lua_pop(L, 1); */
                            /* i++; */
                            const char *controller_type  = luaL_checkstring(L, -2);
                            if (strcmp(controller_type, "keyboard") == 0) {
                                lua_pushnil(L);
                                while(lua_next(L, -2)) {
                                    const char *alias  = luaL_checkstring(L, -2);
                                    memcpy(engine->players.playerInfo[player_id].keyboard_controls[ki].alias, alias, strlen(alias));

                                    lua_getfield(L, -1, "key");
                                    const char *key  = luaL_checkstring(L, -1);
                                    lua_pop(L, 1);

                                    memcpy(engine->players.playerInfo[player_id].keyboard_controls[ki].key, key, strlen(key));

                                    lua_getfield(L, -1, "trigger");
                                    bool trigger = (bool)lua_toboolean(L, -1);
                                    lua_pop(L, 1);

                                    engine->players.playerInfo[player_id].keyboard_controls[ki].keyTriggered = trigger;

                                    lua_getfield(L, -1, "keyrepeat");
                                    bool repeat = (bool)lua_toboolean(L, -1);
                                    engine->players.playerInfo[player_id].keyboard_controls[ki].repeat = repeat;
                                    lua_pop(L, 1);

                                    engine->players.playerInfo[player_id].keyboard_controls[ki].isPressed = false;
                                    lua_pop(L, 1);
                                    ki++;
                                }
                            }

                            if (strcmp(controller_type, "gamepad") == 0) {
                                lua_pushnil(L);
                                while(lua_next(L, -2)) {
                                    const char *alias  = luaL_checkstring(L, -2);
                                    memcpy(engine->players.playerInfo[player_id].gamepad_controls[gi].alias, alias, strlen(alias));

                                    lua_getfield(L, -1, "key");
                                    const char *key  = luaL_checkstring(L, -1);
                                    lua_pop(L, 1);

                                    memcpy(engine->players.playerInfo[player_id].gamepad_controls[gi].key, key, strlen(key));

                                    lua_getfield(L, -1, "trigger");
                                    bool trigger = (bool)lua_toboolean(L, -1);
                                    lua_pop(L, 1);

                                    engine->players.playerInfo[player_id].gamepad_controls[gi].keyTriggered = trigger;

                                    lua_getfield(L, -1, "keyrepeat");
                                    bool repeat = (bool)lua_toboolean(L, -1);
                                    engine->players.playerInfo[player_id].gamepad_controls[gi].repeat = repeat;
                                    lua_pop(L, 1);

                                    engine->players.playerInfo[player_id].gamepad_controls[gi].isPressed = false;
                                    lua_pop(L, 1);
                                    gi++;
                                }
                            }

                            lua_pop(L, 1);
                        }

                        lua_pop(L, 1);
                    }
                    engine->players.playerInfo[player_id].keyboard_controls_length = ki;
                    engine->players.playerInfo[player_id].gamepad_controls_length = gi;
                    printf("lua player players.playerInfo[%d].keyboard_controls_length: %d\n", player_id, engine->players.playerInfo[player_id].keyboard_controls_length);
                    printf("lua player players.playerInfo[%d].gamepad_controls_length: %d\n", player_id, engine->players.playerInfo[player_id].gamepad_controls_length);
                }
            }

            if (!strcmp(option, "assign_controllers")) {
                    lua_pushnil(L);
                    uint8_t count = 0;
                    while(lua_next(L, -2)) {

                        uint8_t i = (uint8_t)luaL_checknumber(L, -2);

                        lua_getfield(L, -1, "controller");
                        const char *controller  = luaL_checkstring(L, -1);
                        lua_pop(L, 1);

                        lua_getfield(L, -1, "id");
                        uint8_t id = luaL_checknumber(L, -1);
                        lua_pop(L, 1);
                        i -= 1;
                        /* printf("%d:  %s\n", i, type); */
                        if (strcmp(controller, "none") == 0) {
                            /* engine->player_control_type_flags |= (1 << i); */
                            /* engine->players.playerInfo[i].controller_type = PLAYER_CONTOLLER_KEYBOARD; */
                            engine->players.playerInfo[i].controller_id = -1;
                        } else {
                            int controller_id = -1;

                            char connection_str[500];
                            memset(connection_str, 0, 500);
                            for (int i = 0; i < SDL_NumJoysticks(); ++i){
                                const char *name = SDL_GameControllerNameForIndex(i);
                                sprintf(connection_str, "%d: %s", i, name);
                                if (strcmp(controller, connection_str) == 0) {
                                    controller_id = i;
                                    break;
                                }
                            }
                            printf("controller id: %d\n", controller_id);
                            engine->players.playerInfo[i].controller_id = controller_id;
                            /* engine->players.playerInfo[i].controller_type = PLAYER_CONTOLLER_GAMEPAD; */
                        }

                        lua_pop(L, 1);
                        count++;
                    }
                
            }
           lua_pop(L, 1);
           i++;
        }
 
    }

    /* printf("player_control_type_flags: %u\n", engine->player_control_type_flags); */
    engine->frame_rate = frame_rate;
    /* if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO)) { */
    /*     N2DE_ERROR("could not initialize SDL\n%s", SDL_GetError()); */
    /*     exit(1); */
    /* } */
    printf("controllers:  \n");
    for (int i = 0; i < SDL_NumJoysticks(); ++i){
        printf("%d controller: '%s' \n", i, SDL_GameControllerNameForIndex(i));
    }
    printf("\n");
    for(int i = 0; i < engine->players.count; ++i){
        if (engine->players.playerInfo[i].controller_id >= 0) {
            engine->players.playerInfo[i].controller = \
                SDL_GameControllerOpen(engine->players.playerInfo[i].controller_id);
                printf("connecting to conntroller: %d\n", engine->players.playerInfo[i].controller_id);

        }
    }
    engine->window_rect.x = p.x;
    engine->window_rect.y = p.y;
    engine->window_rect.w = s.w;
    engine->window_rect.h = s.h;
    engine->window_scale = scale;

    engine->window = SDL_CreateWindow(windowName, p.x, p.y, s.w * scale,
                                      s.h * scale, 0);
    SDL_SetWindowFullscreen(engine->window, isFullscreen);
    SDL_ShowCursor(SDL_DISABLE);
    if (!engine->window) {
        N2DE_ERROR("could not creat a window\n%s", SDL_GetError());
    }

    engine->renderer = SDL_CreateRenderer(engine->window, -1,\
                                           SDL_RENDERER_PRESENTVSYNC);
    /* SDL_SetRenderDrawBlendMode(engine->renderer, SDL_BLENDMODE_NONE); */
    if (!engine->renderer) {
        N2DE_ERROR("could not create renderer\n%s", SDL_GetError());
    }

    int ret = SDL_RenderSetLogicalSize(engine->renderer, s.w, s.h);

    if (ret < 0) {
        N2DE_ERROR("could not set logical size\n%s", SDL_GetError());
    }

    SDL_SetRenderDrawColor(engine->renderer, 0, 0, 0, 255);
    SDL_RenderClear(engine->renderer);
    terminal_init(&engine->terminal, term_max_history, term_font_size, term_height);
    terminal_changeFgColor(&engine->terminal, &term_fg_color);
    terminal_changeBgColor(&engine->terminal, &term_bg_color);
    terminal_changeCMDColor(&engine->terminal, &term_cmd_color);
    terminal_changeErrorColor(&engine->terminal, &term_error_color);
    engine->initialized = true;

    loop_id = loops_add(&engine->loops, engine->frame_rate);

    double deltaTime = (double)engine->loops.loopInfo[loop_id].mspl / (double)1000;
    engine->deltaTime = deltaTime;
    lua_getglobal(engine->lua_state, "nyq");
    lua_pushnumber(engine->lua_state, deltaTime);
    lua_setfield(engine->lua_state, -2, "deltaTime");

    for (int i = 0; i < engine->players.playerInfo[0].keyboard_controls_length; ++i) {
        PlayerInfo *player = &engine->players.playerInfo[0];
            player->keyboard_controls[i].previously_pressed = false;
            player->keyboard_controls[i].triggered = true;
            player->gamepad_controls[i].previously_pressed = false;
            player->gamepad_controls[i].triggered = true;
    }
    return 0;
}

int luaGlobal_contollsReset(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    for (int i = 0; i < engine->players.playerInfo[0].keyboard_controls_length; ++i) {
        PlayerInfo *player = &engine->players.playerInfo[0];
            player->keyboard_controls[i].previously_pressed = false;
            player->keyboard_controls[i].triggered = true;
            player->gamepad_controls[i].previously_pressed = false;
            player->gamepad_controls[i].triggered = true;
    }

}

int luaGlobal_clear(lua_State *L)
{

    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    SDL_Color color;
    lua_tocolor(L, 1, &color);
    SDL_SetRenderDrawColor(engine->renderer, color.r, color.g, color.b, 
                           color.a);

    SDL_RenderClear(engine->renderer);
    return 0;
}

int luaGlobal_drawCircle(lua_State *L)
{
    int arg_count = lua_gettop(L);
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    SDL_Point center;
    SDL_Point tilt = {1, 1};
    SDL_Color color;
    lua_topoint(L, 1, &center);
    lua_tocolor(L, 2, &color);
    int radius = (int)luaL_checknumber(L, 3);

    if (arg_count == 4) {
        lua_topoint(L, 4, &tilt);
    }
    circles_create(&engine->circles, &color, &center, radius, &tilt);
    return 0;
}

int luaGlobal_getTicks(lua_State *L)
{
    uint64_t ticks= SDL_GetTicks64();
    lua_pushnumber(L, ticks);
    return 1;
}

static void getKeyboardControlls(Nyquist2DEngine *engine)
{
    uint8_t *keystate = 0;
    engine->keyinfo.state = NULL;
    SDL_Keycode key = 0;
    bool terminal_key = false;
    while (SDL_PollEvent(&engine->event)) {
        keystate = (uint8_t *)SDL_GetKeyboardState(NULL);
        engine->keyinfo.state = keystate;
        key = engine->event.key.keysym.sym;

        switch (engine->event.type) {
        case SDL_KEYDOWN: 
        {
            engine->keyinfo.key = key;
            engine->keyinfo.isDown = true;
            if (key == SDLK_BACKQUOTE && keystate[SDL_SCANCODE_LCTRL]) {
                terminal_key = true;
            }
            if (engine->terminal.toggled) break;
            for (int i = 0; i < engine->players.playerInfo[0].keyboard_controls_length; ++i) {

                if (!strcmp(engine->players.playerInfo[0].keyboard_controls[i].key, 
                            SDL_GetKeyName(key))) {
                    engine->players.playerInfo[0].keyboard_controls[i].isPressed = true;
                    if (engine->players.playerInfo[0].keyboard_controls[i].repeat)
                        engine->players.playerInfo[0].keyboard_controls[i].triggered = false;
                }
            }
            break;
        } // case
        case SDL_KEYUP:
        {
            engine->keyinfo.isDown = false;
            engine->keyinfo.state = NULL;
            engine->keyinfo.key = 0;
            if (engine->terminal.toggled) break;
            for (int i = 0; i < engine->players.playerInfo[0].keyboard_controls_length; ++i) {
                if (!strcmp(engine->players.playerInfo[0].keyboard_controls[i].key, 
                            SDL_GetKeyName(key))) {
                    engine->players.playerInfo[0].keyboard_controls[i].isPressed = false;

                    if (!engine->players.playerInfo[0].keyboard_controls[i].repeat)
                        engine->players.playerInfo[0].keyboard_controls[i].triggered = false;
                }
            }
            break;
        } // case
        case SDL_MOUSEWHEEL:
        {
            if (engine->terminal.toggled) {
                if (engine->event.wheel.y > 0) { // scroll up
                    engine->terminal.scroll--;
                }
                if (engine->event.wheel.y < 0) { // scroll down
                    engine->terminal.scroll++;
                }
            }
            break;
        } // case
        case SDL_QUIT:
        {
            engine->quit = true;
            break;
        } // case
        } // switch
    }
    if (terminal_key) {
        terminal_toggle(&engine->terminal, engine->window_rect.w,
                        engine->window_rect.h);
    }
    if (engine->terminal.toggled) {
        terminal_updateKeys(&engine->terminal, engine->lua_state,
                            &engine->keyinfo);
    }
}
static void getGamepadControlls(Nyquist2DEngine *engine)
{
    uint8_t *keystate = 0;
    engine->keyinfo.state = NULL;
    SDL_Keycode key = 0;
    bool terminal_key = false;
    for (int i = 0; i < engine->players.playerInfo[0].gamepad_controls_length; ++i) {
        PlayerInfo *player = &engine->players.playerInfo[0];
        player->gamepad_controls[i].isPressed = \
            SDL_GameControllerGetButton(player->controller, 
            SDL_GameControllerGetButtonFromString(player->gamepad_controls[i].key));
        if (!player->gamepad_controls[i].isPressed && player->gamepad_controls[i].previously_pressed) {
            // KEYUP!
            if (!engine->players.playerInfo[0].gamepad_controls[i].repeat)
                engine->players.playerInfo[0].gamepad_controls[i].triggered = false;
            player->gamepad_controls[i].previously_pressed = false;
        } 

        if (player->gamepad_controls[i].isPressed){
            if (engine->players.playerInfo[0].gamepad_controls[i].repeat)
                engine->players.playerInfo[0].gamepad_controls[i].triggered = false;
            player->gamepad_controls[i].previously_pressed = true;
        }
    }
    while (SDL_PollEvent(&engine->event)) {
        keystate = (uint8_t *)SDL_GetKeyboardState(NULL);
        engine->keyinfo.state = keystate;
        key = engine->event.key.keysym.sym;
        switch (engine->event.type) {
            case SDL_KEYDOWN: 
            {
                engine->keyinfo.key = key;
                engine->keyinfo.isDown = true;
                if (key == SDLK_BACKQUOTE && keystate[SDL_SCANCODE_LCTRL]) {
                    terminal_key = true;
                }
                if (engine->terminal.toggled) break;
                break;
            } // case
            case SDL_KEYUP:
            {
                engine->keyinfo.isDown = false;
                engine->keyinfo.state = NULL;
                engine->keyinfo.key = 0;
                if (engine->terminal.toggled) break;
                break;
            } // case
            case SDL_MOUSEWHEEL:
            {
                if (engine->terminal.toggled) {
                    if (engine->event.wheel.y > 0) { // scroll up
                        engine->terminal.scroll--;
                    }
                    if (engine->event.wheel.y < 0) { // scroll down
                        engine->terminal.scroll++;
                    }
                }
                break;
            } // case
            case SDL_QUIT:
            {
                engine->quit = true;
                break;
            } // case
        }
    }
    if (terminal_key) {
        terminal_toggle(&engine->terminal, engine->window_rect.w,
                        engine->window_rect.h);
    }
    if (engine->terminal.toggled) {
        terminal_updateKeys(&engine->terminal, engine->lua_state,
                            &engine->keyinfo);
    }
}

int luaGlobal_controlsUpdate(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    if (!engine->initialized) return 0;

    switch ((engine->players.playerInfo[0].controller_id >= 0) ? 1 : 0){
        case 0: getKeyboardControlls(engine); break;
        case 1:  getGamepadControlls(engine); break;
    }

    lua_getglobal(L, "nyq");
    lua_pushstring(L, "quit");
    lua_pushboolean(L, engine->quit);
    lua_settable(L, -3);
    if (engine->keyinfo.isDown) {
        char str[50];
        memset(str, 0, 50);
        strcat(str, SDL_GetKeyName(engine->keyinfo.key));
        /* str[0] = '\0'; */
        char key[2] = "";
        /* if (strcmp(str, "Space") == 0) { */
        /*     printf("hit space\n"); */
        /* } */
        bool hitSpace = strcmp(str, "Space") == 0;
        bool hitBackSpace = false;
        if (engine->keyinfo.state) {
            hitBackSpace = engine->keyinfo.state[SDL_SCANCODE_BACKSPACE];
        }
        if ((strlen(str) <= 2 || hitSpace || hitBackSpace) && !engine->terminal.toggled
            && str[0] != '`') {
            key[0] = str[0];
            if (hitBackSpace) {
                key[0] = '\b';
            }
            else if (hitSpace) {
                key[0] = ' ';
            } 
            else if (engine->keyinfo.state[SDL_SCANCODE_LSHIFT]) {
                if (engine->keyinfo.state[SDL_SCANCODE_1]) {
                    key[0] = '!';
                }
                else if (engine->keyinfo.state[SDL_SCANCODE_2]) {
                    key[0] = '@';
                }
                else if (engine->keyinfo.state[SDL_SCANCODE_3]) {
                    key[0] = '#';
                }
                else if (engine->keyinfo.state[SDL_SCANCODE_4]) {
                    key[0] = '$';
                }
                else if (engine->keyinfo.state[SDL_SCANCODE_5]) {
                    key[0] = '%';
                }
                else if (engine->keyinfo.state[SDL_SCANCODE_6]) {
                    key[0] = '^';
                }
                else if (engine->keyinfo.state[SDL_SCANCODE_7]) {
                    key[0] = '&';
                }
                else if (engine->keyinfo.state[SDL_SCANCODE_8]) {
                    key[0] = '*';
                }
                else if (engine->keyinfo.state[SDL_SCANCODE_9]) {
                    key[0] = '(';
                }
                else if (engine->keyinfo.state[SDL_SCANCODE_0]) {
                    key[0] = ')';
                }
                else if (engine->keyinfo.state[SDL_SCANCODE_MINUS]) {
                    key[0] = '_';
                }
                else if (engine->keyinfo.state[SDL_SCANCODE_EQUALS]) {
                    key[0] = '+';
                }
                else if (engine->keyinfo.state[SDL_SCANCODE_COMMA]) {
                    key[0] = '<';
                }
                else if (engine->keyinfo.state[SDL_SCANCODE_PERIOD]) {
                    key[0] = '>';
                }
                else if (engine->keyinfo.state[SDL_SCANCODE_SLASH]) {
                    key[0] = '?';
                }
                else if (engine->keyinfo.state[SDL_SCANCODE_BACKSLASH]) {
                    key[0] = '|';
                }
                else if (engine->keyinfo.state[SDL_SCANCODE_LEFTBRACKET]) {
                    key[0] = '{';
                }
                else if (engine->keyinfo.state[SDL_SCANCODE_RIGHTBRACKET]) {
                    key[0] = '}';
                }
                else if (engine->keyinfo.state[SDL_SCANCODE_SEMICOLON]) {
                    key[0] = ':';
                }
                else if (engine->keyinfo.state[SDL_SCANCODE_APOSTROPHE]) {
                    key[0] = '"';
                }

            }
            else  {
                key[0] = tolower(str[0]);
            }
            strcat(key, "\0");
            lua_pushstring(L, key);
            lua_setfield(L, -2, "key");
            engine->keyinfo.isDown = false;
        }
    } else {
        lua_pushnil(L);
        lua_setfield(L, -2, "key");

    }

    lua_getfield(L, -1, "keypress");

    bool trigger = false;
    lua_getfield(L, -1, "trigger");

    if (lua_isnil(engine->lua_state, -1)) {
        trigger = false;
    } else {
        trigger = (bool)lua_toboolean(engine->lua_state, -1);
    }
    lua_pop(L, 1);


    if(engine->players.playerInfo[0].controller_id == -1) {

        for (int i = 0; i < engine->players.playerInfo[0].keyboard_controls_length; ++i) {
            lua_getfield(L, -1, engine->players.playerInfo[0].keyboard_controls[i].alias);
            bool lua_ispressed = (bool)lua_toboolean(L, -1);
            lua_pop(L, 1);

            lua_pushstring(L, engine->players.playerInfo[0].keyboard_controls[i].alias);
            bool isPressed = engine->players.playerInfo[0].keyboard_controls[i].isPressed;

            if (trigger || engine->players.playerInfo[0].keyboard_controls[i].keyTriggered) {
                isPressed = false;
                if (!engine->players.playerInfo[0].keyboard_controls[i].triggered) {
                    isPressed = true;
                    engine->players.playerInfo[0].keyboard_controls[i].triggered = true;
                }

            }
            lua_pushboolean(L, isPressed);
            lua_settable(L, -3);
        }
    }else{

        for (int i = 0; i < engine->players.playerInfo[0].gamepad_controls_length; ++i) {
            lua_getfield(L, -1, engine->players.playerInfo[0].gamepad_controls[i].alias);
            bool lua_ispressed = (bool)lua_toboolean(L, -1);
            lua_pop(L, 1);

            lua_pushstring(L, engine->players.playerInfo[0].gamepad_controls[i].alias);
            bool isPressed = engine->players.playerInfo[0].gamepad_controls[i].isPressed;

            if (trigger || engine->players.playerInfo[0].gamepad_controls[i].keyTriggered) {
                isPressed = false;
                if (!engine->players.playerInfo[0].gamepad_controls[i].triggered) {
                    isPressed = true;
                    engine->players.playerInfo[0].gamepad_controls[i].triggered = true;
                }

            }
            lua_pushboolean(L, isPressed);
            lua_settable(L, -3);
        }
    }
    /* SDL_Delay(.5); */

    return 0;
}

int luaGlobal_FrameCapEnable(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    bool hasFrameCap = lua_toboolean(L, 1);
    engine->hasFrameCap = hasFrameCap;
    return 0;
}


int luaGlobal_drawUpdate(lua_State *L)
{

    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    /* loops_start(&engine->loops, loop_id); */
    SDL_SetRenderDrawBlendMode(engine->renderer, SDL_BLENDMODE_NONE);
    /* SDL_SetRenderDrawBlendMode(engine->renderer, SDL_BLENDMODE_BLEND); */

    uint16_t ei = 0;
    SDL_Rect rect = {
        .x = 0,
        .y = 0,
        .w = engine->window_rect.w,
        .h = engine->window_rect.h,
    };
    world_updateQue(&engine->world, engine->renderer, &engine->images, 
                    &engine->sprites, &engine->players,
                    engine->world.map.position, &rect);
    images_updateQue(&engine->images, engine->update_elements, &ei);
    sprites_updateQue(&engine->sprites, engine->update_elements, &ei);
    fonts_updateQue(&engine->fonts, engine->update_elements, &ei);
    SDL_SetRenderDrawBlendMode(engine->renderer, SDL_BLENDMODE_BLEND);
    rects_updateQue(engine->update_elements, &ei);
    lines_updateQue(&engine->lines, engine->update_elements, &ei);

    screenElements_update(engine->update_elements, ei, engine->renderer, &rect);

    /* circles_update(&engine->circles, engine->renderer); */
    terminal_update(&engine->terminal, engine->lua_state, engine->renderer,
                    engine->window_scale);
    /* waiting_update(&engine->waiting); */
    SDL_RenderPresent(engine->renderer);

    fonts_reset(&engine->fonts);
    sprites_reset(&engine->sprites);
    images_reset(&engine->images);
    rects_reset();
    lines_reset(&engine->lines);

    if (engine->hasFrameCap && (!engine->terminal.toggled)) {
        /* loops_end(&engine->loops, loop_id); */
    }  
    return 0;
}

int luaGlobal_quit(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    engine->quit = true;
    return 0;
}

/* int luaGlobal_gamepadIsConnected(lua_State *L) */
/* { */
/*     Nyquist2DEngine *engine = NULL; */
/*     LUA_GETENGINE(L, engine); */
/*     lua_pushboolean(L, SDL_NumJoysticks()); */
/*     return 1; */
/* } */

int luaGlobal_gamepadChange(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int id = luaL_checknumber(L, 1);
    const char * controller_name = luaL_checkstring(L, 2);

    char connection_str[500];
    memset(connection_str, 0, 500);
    engine->players.playerInfo[id].controller_id = -1;

    if (strcmp(controller_name, "none") != 0) {
        int controller_id = -1;

        printf("gamepad search for '%s'\n", controller_name);
        for (int i = 0; i < SDL_NumJoysticks(); ++i){
            const char *name = SDL_GameControllerNameForIndex(i);
            /* strcat(connection_str, name); */
            sprintf(connection_str, "%d: %s", i, name);

            printf("found '%s'\n", name);
            if (strcmp(connection_str, controller_name) == 0) {
                printf("matched '%s'\n", name);
                controller_id = i;
                break;
            }
        }

        if (controller_id < 0) return 0;
        engine->players.playerInfo[id].controller_id = controller_id;

        printf("connecting to %s\n", controller_name);
        engine->players.playerInfo[id].controller = \
            SDL_GameControllerOpen(controller_id);

    }

    return 0;
}

int luaGlobal_getGamepads(lua_State *L)
{
    int i = 0;
    char connection_str[500];
    memset(connection_str, 0, 500);
    lua_newtable(L);
    for (; i < SDL_NumJoysticks(); ++i){
        const char *name = SDL_GameControllerNameForIndex(i);
        lua_pushnumber(L, i + 1);
        /* lua_pushstring(L, name); */
        sprintf(connection_str, "%d: %s", i, name);
        lua_pushstring(L, connection_str);
        lua_settable(L, -3);
    }
    if (i == 0) lua_pushnil(L);

    return 1;
}

/* int luaGlobal_openGamepad(lua_State *L) */
/* { */
/*     Nyquist2DEngine *engine = NULL; */
/*     LUA_GETENGINE(L, engine); */
/*     int id = luaL_checknumber(L, 1); */
/*     const char *controller_name = luaL_checkstring(L, 2); */
/*     int controller_id = -1; */

/*     for (int i = 0; i < SDL_NumJoysticks(); ++i){ */
/*         const char *name = SDL_GameControllerNameForIndex(i); */
/*         if (strcmp(controller_name, name) == 0) { */
/*             controller_id = i; */
/*             break; */
/*         } */
/*     } */
/*     if (engine->players.playerInfo[id].controller_id < 0) return 0; */
/*     SDL_GameControllerOpen(controller_id); */
/*     engine->players.playerInfo[id].gamepad_isConnected = true; */
/*     return 0; */
/* } */

const struct luaL_Reg luaFunctions_global[] = {
    {"delay", luaGlobal_delay},
    {"init", luaGlobal_init},
    {"clear", luaGlobal_clear},
    {"drawCircle", luaGlobal_drawCircle},
    {"getTicks", luaGlobal_getTicks},
    {"controlsUpdate", luaGlobal_controlsUpdate},
    {"draw", luaGlobal_drawUpdate},
    {"controlsReset", luaGlobal_contollsReset},
    {"exit", luaGlobal_quit},
    {"hasFrameCap", luaGlobal_FrameCapEnable},
    /* {"gamepadIsConnected", luaGlobal_gamepadIsConnected}, */
    {"controllerChange", luaGlobal_gamepadChange},
    {"getGamepads", luaGlobal_getGamepads},
    /* {"openGamepad", luaGlobal_openGamepad}, */
    {NULL, NULL}
};

#endif // N2DE_LUA_GLOBALS_H_
