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
#ifndef N2DE_TERMINAL_H_
#define N2DE_TERMINAL_H_

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../lua5.3/lua.h"
#include "../lua5.3/lauxlib.h"
#include "../lua5.3/lualib.h"

#include "./rects.h"
#include "../SDL2/SDL_ttf.h"
#include "./config.h"

#define TERM_MSGNORM 0
#define TERM_MSGERROR 1
#define TERM_MSGCMD 2
typedef struct _Message {
    char *str;
    int type;
    // 0 -> normal
    // 1 -> error
} Message;

typedef struct _Terminal {
    SDL_Rect rect;
    Size window_size;
    bool toggled;
    SDL_Color bg_color;
    SDL_Color fg_color;
    SDL_Color error_color;
    SDL_Color cmd_color;
    int height;
    TTF_Font *ttf;
    char **history;
    uint16_t max_history;
    uint8_t history_len;
    char command[255];
    int command_pos;
    int font_size;
    int command_offset;
    Message *messages;
    uint16_t messages_len;
    SDL_Surface *surface;
    SDL_Texture *texture;
    int scroll;
    bool execute;
} Terminal;

SDL_Rect _getTextRect(Terminal *term, SDL_Surface *surface, int scale, int xoffset, int yoffset, bool canScroll) {
    int scroll = canScroll ? term->scroll : 0;
    SDL_Rect r;
    r.x = xoffset;
    r.y = (term->window_size.h - (surface->h / scale) * (yoffset + 1)) + scroll;
    r.w = surface->w / scale;
    r.h = surface->h / scale;
    return r;
}


long int _linesWrap(char *str, int charWidth, int maxWidth)
{
    int len = strlen(str);
    if (len <= maxWidth) return -1;

    while(len > maxWidth) len--;
    return len;
}

bool checkIfLuaString(char *cmd)
{
    if (strlen(cmd) < 4) return false;
    char *cmp = "lua ";

    int i = 0;
    for (; *cmd != '\0' && i < 4; cmd++, i++) {
        if (*(cmp + i) != *cmd) return false;
    }

    return true;
}

void terminal_init(Terminal *term, uint16_t max_history, int fontSize, int terminal_h)
{
    memset(term, 0, sizeof(Terminal));
    term->ttf = TTF_OpenFont(RESPATH "/fonts/NotoSansMono-Regular.ttf", fontSize);
    if (!term->ttf) {
        N2DE_ERROR("could not open terminal font\n");
        printf("%s\n", RESPATH "/fonts/NotoSansMono-Regular.ttf");
    }
    term->max_history = max_history;
    term->history = malloc(sizeof(char *) * term->max_history);
    for (int i = 0; i < term->max_history; ++i) {
        term->history[i] = malloc(sizeof(char) * 255);
    }
    term->execute = false;
    term->history_len = 0;
    term->toggled = false;
    term->height = terminal_h;
    term->bg_color.r = 0;
    term->bg_color.g = 0;
    term->bg_color.b = 0;
    term->bg_color.a = 200;

    term->fg_color.r = 255;
    term->fg_color.g = 255;
    term->fg_color.b = 255;
    term->fg_color.a = 255;

    term->error_color.r = 255;
    term->error_color.g = 0;
    term->error_color.b = 0;
    term->error_color.a = 255;

    term->cmd_color.r = 255;
    term->cmd_color.g = 255;
    term->cmd_color.b = 0;
    term->cmd_color.a = 255;
    term->messages = NULL;
    term->messages_len = 0;
}

void terminal_changeFgColor(Terminal *term, SDL_Color *color) {
    memcpy(&term->fg_color, color, sizeof(SDL_Color));
}

void terminal_changeBgColor(Terminal *term, SDL_Color *color) {
    memcpy(&term->bg_color, color, sizeof(SDL_Color));
}

void terminal_changeCMDColor(Terminal *term, SDL_Color *color) {
    memcpy(&term->cmd_color, color, sizeof(SDL_Color));
}

void terminal_changeErrorColor(Terminal *term, SDL_Color *color) {
    memcpy(&term->error_color, color, sizeof(SDL_Color));
}

// TODO:
// make this pause the gameloop
void terminal_toggle(Terminal *term, int window_w, int window_h)
{
    term->toggled = !term->toggled;
    if (!term->toggled) return;
    term->rect.w = window_w;
    term->window_size.w = window_w;
    term->window_size.h = window_h;
    term->rect.x = 0;
}

void terminal_historyOffset(Terminal *term, int offset) {
    bool go = true;
    if (offset > 0) {
        go = term->command_offset + offset <= 0;

    } else if (offset < 0){
        go = term->command_offset + offset >= -(term->history_len);
    }

    if (!go) return;
    term->command_offset += offset;
    if (term->command_offset != 0) {
        char *old_command = term->history[term->history_len +
                            term->command_offset];
        int len = strlen(old_command);
        memset(term->command, 0, 255);
        memcpy(term->command, old_command, len + 1);
        term->command_pos = len;
    }  else {
        memset(term->command, 0, 255);
        term->command_pos = 0;
    }
}

int terminal_addMessage(Terminal *term, char *msg, int type)
{
    /* terminal_addMessage(term, term->command, TERM_MSGNORM); */
    int len = strlen(msg);
    int i = term->messages_len++;
    term->messages = realloc(term->messages,
                             sizeof(Message) *
                             term->messages_len);

    term->messages[i].str = malloc(len + 1);
    memcpy(term->messages[i].str, msg, len);
    term->messages[i].str[len] = '\0';
    term->messages[i].type = type;
    return i;
}


void terminal_updateKeys(Terminal *term, lua_State *L, KeyInfo *keyinfo)
{
    if (!term->toggled) return;
    uint8_t *keystate = keyinfo->state;
    SDL_Keycode key = keyinfo->key;
    bool keydown = keyinfo->isDown;
    /* term->execute = false; */

    if (keystate && keydown) {

        bool isShift = (keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT]);

        if (!(keystate[SDL_SCANCODE_GRAVE] && keystate[SDL_SCANCODE_LCTRL])) {
            if (key == SDLK_RETURN) {
                term->execute = true;
                term->command_offset = 0;
            } 
            else if(keystate[SDL_SCANCODE_BACKSPACE]) {
                if (term->command_pos > 0) {
                    term->command_pos--;
                }
                term->command[term->command_pos] = '\0';
            }
            else if(keystate[SDL_SCANCODE_LCTRL] && keystate[SDL_SCANCODE_P]) {
                terminal_historyOffset(term, -1);
            }
            else if(keystate[SDL_SCANCODE_LCTRL] && keystate[SDL_SCANCODE_N]) {
                terminal_historyOffset(term, +1);
            }
            else if(keystate[SDL_SCANCODE_LCTRL] && keystate[SDL_SCANCODE_L]) {
                term->scroll = 0;
            }
            else if(keystate[SDL_SCANCODE_LCTRL] && keystate[SDL_SCANCODE_U]) {
                memset(term->command, 0, 255);
                term->command_pos = 0;
                term->scroll = 0;
            }
            else if(keystate[SDL_SCANCODE_LCTRL] && keystate[SDL_SCANCODE_W]) {
                char *p = term->command;
                int len = strlen(term->command);
                p += len - 1;

                if (isalpha(*p) == 0) {
                    while((isalpha(*p) == 0) &&  (p - term->command >= 0)) {
                        *p = '\0';
                        p--;
                    }
                } else {

                    while((isalpha(*p) > 0) &&  (p - term->command >= 0)) {
                        *p = '\0';
                        p--;
                    }
                }


                term->command_pos = strlen(term->command);
            }
            else 
            {
                if (key == SDLK_SPACE) {
                    term->command[term->command_pos] = ' ';
                    term->command_pos++;
                }

                for (int c = 'a'; c <= 'z'; ++c) {
                    int sdlk = SDLK_a + (c - 'a');
                    if (c == sdlk && key == sdlk) { 
                        term->command[term->command_pos] = isShift ? c + ('A' - 'a'): c;
                        term->command_pos++;
                    }
                }

                if (key == SDLK_BACKQUOTE) {
                    term->command[term->command_pos] = isShift ? '~' : '`'; 
                    term->command_pos++;
                }
                else if (key == SDLK_1) {
                    term->command[term->command_pos] = isShift ? '!' : '1'; 
                    term->command_pos++;
                }
                else if (key == SDLK_2) {
                    term->command[term->command_pos] = isShift ? '@' : '2'; 
                    term->command_pos++;
                }
                else if (key == SDLK_3) {
                    term->command[term->command_pos] = isShift ? '#' : '3'; 
                    term->command_pos++;
                }
                else if (key == SDLK_4) {
                    term->command[term->command_pos] = isShift ? '$' : '4'; 
                    term->command_pos++;
                }
                else if (key == SDLK_5) {
                    term->command[term->command_pos] = isShift ? '%' : '5'; 
                    term->command_pos++;
                }
                else if (key == SDLK_6) {
                    term->command[term->command_pos] = isShift ? '^' : '6'; 
                    term->command_pos++;
                }
                else if (key == SDLK_7) {
                    term->command[term->command_pos] = isShift ? '&' : '7'; 
                    term->command_pos++;
                }
                else if (key == SDLK_8) {
                    term->command[term->command_pos] = isShift ? '*' : '8'; 
                    term->command_pos++;
                }
                else if (key == SDLK_9) {
                    term->command[term->command_pos] = isShift ? '(' : '9'; 
                    term->command_pos++;
                }
                else if (key == SDLK_0) {
                    term->command[term->command_pos] = isShift ? ')' : '0'; 
                    term->command_pos++;
                }
                else if (key == SDLK_MINUS) {
                    term->command[term->command_pos] = isShift ? '_' : '-'; 
                    term->command_pos++;
                }
                else if (key == SDLK_EQUALS) {
                    term->command[term->command_pos] = isShift ? '+' : '='; 
                    term->command_pos++;
                }
                else if (key == SDLK_LEFTBRACKET) {
                    term->command[term->command_pos] = isShift ? '{' : '['; 
                    term->command_pos++;
                }
                else if (key == SDLK_RIGHTBRACKET) {
                    term->command[term->command_pos] = isShift ? '}' : ']'; 
                    term->command_pos++;
                }
                else if (key == SDLK_BACKSLASH) {
                    term->command[term->command_pos] = isShift ? '|' : '\\'; 
                    term->command_pos++;
                }
                else if (key == SDLK_SEMICOLON) {
                    term->command[term->command_pos] = isShift ? ':' : ';'; 
                    term->command_pos++;
                }
                else if (key == SDLK_QUOTE) {
                    term->command[term->command_pos] = isShift ? '"' : '\''; 
                    term->command_pos++;
                }
                else if (key == SDLK_COMMA) {
                    term->command[term->command_pos] = isShift ? '<' : ','; 
                    term->command_pos++;
                }
                else if (key == SDLK_PERIOD) {
                    term->command[term->command_pos] = isShift ? '>' : '.'; 
                    term->command_pos++;
                }
                else if (key == SDLK_SLASH) {
                    term->command[term->command_pos] = isShift ? '?' : '/'; 
                    term->command_pos++;
                }
                else if (key == SDLK_TAB) {
                    // get Global variable "_G" and use tab to find the
                    // most close variable
                    lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
                    lua_pushnil(L);
                    while(lua_next(L, -2) != 0) {
                        char *key = (char *)lua_tostring(L, -2);
                        int len = strlen(key);
                        printf("%s\n", key);
                        lua_pop(L, 1);

                    }
                }
            }

        }

    }

}

static char **lines = NULL;
static int lines_count = 0;

void terminal_update(Terminal *term, lua_State *L, SDL_Renderer *renderer,
                     int scale)
{
    if (!term->toggled) return;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, term->bg_color.r, term->bg_color.g,
                           term->bg_color.b, term->bg_color.a);
    SDL_RenderFillRect(renderer, &term->rect);
    term->rect.y = term->window_size.h - (term->height / scale);
    term->rect.h = term->height / scale;

    char ps1[255];

    sprintf(ps1, "> %s|", term->command);

    if (term->execute) {
        char out[255];
        bool isLuaString = checkIfLuaString(term->command);
        sprintf(out, "> %s", term->command);
        terminal_addMessage(term, out, TERM_MSGCMD);

        lua_getglobal(L, "terminal");

        int doStringError = false;
        if (isLuaString) {
            char *cmd = term->command + 4;
            doStringError = luaL_dostring(L, cmd);

        } else if (lua_type(L, -1) == LUA_TTABLE) {
            lua_getfield(L, -1, "commands");
            if (lua_type(L, -1) == LUA_TTABLE) {
                lua_pushnil(L);
                bool found = false;
                while(lua_next(L, -2)) {
                    char *name = (char *)luaL_checkstring(L, -2);
                    lua_pop(L, 1);

                    /* printf("isLuaString: %d\n", isLuaString); */
                    if (strstr(term->command, name) == term->command) {
                        char cmd[512];
                        sprintf(cmd, "terminal.commands.%s", term->command);
                        found = true;
                        doStringError = luaL_dostring(L, cmd);
                    }  
                }
                if (!found) {
                    terminal_addMessage(term, "not a command", TERM_MSGERROR);
                }
            }
        }

        /* int doStringError = true; */

        if (doStringError) {
            char *msg = (char *)lua_tostring(L, -1);
            terminal_addMessage(term, msg, TERM_MSGERROR);
        } else {
            if (term->history_len >= term->max_history) {
                for (int i = 0; i < term->max_history - 1; ++i) {
                    int len = strlen(term->history[i + 1]);
                    memset(term->history[i], 0, 255);
                    memcpy(term->history[i], term->history[i + 1], len + 1);
                }
                term->history_len  = term->max_history - 1;
            }

            memset(term->history[term->history_len], 0, 255);
            memcpy(term->history[term->history_len], term->command, strlen(term->command));
            term->history_len++;
        }
        memset(term->command, 0, 255);
        term->command_pos = 0;
        term->scroll = 0;

        term->execute = false;
    }


    term->surface = \
        TTF_RenderText_Blended(term->ttf, ps1, term->fg_color);

    SDL_Rect r = _getTextRect(term, term->surface, scale, 0, 0, false);

    int extra = r.w - term->window_size.w;
    if (extra > 0) {
        r.x -= extra;
    }

    term->texture = \
        SDL_CreateTextureFromSurface(renderer, term->surface);

    SDL_RenderCopy(renderer, term->texture, NULL, &r);
    SDL_FreeSurface(term->surface);
    term->surface = NULL;
    SDL_DestroyTexture(term->texture);
    term->texture = NULL;
    for (int i = term->messages_len, j = 1; i > 0; --i, j++) {
        Message message = term->messages[i - 1];
        SDL_Color color = term->fg_color;
        int message_len = strlen(message.str);
        switch(message.type) {
        case TERM_MSGNORM:  color = term->fg_color; break;
        case TERM_MSGERROR: color = term->error_color; break;
        case TERM_MSGCMD: color = term->cmd_color; break;
        }

        term->surface = \
            TTF_RenderText_Blended(term->ttf, message.str, color);


        int charWidth = (term->surface->w / message_len) / scale;
        SDL_FreeSurface(term->surface);
        term->surface = NULL;


        char *str = message.str;

        int maxChars  = term->window_size.w / charWidth;

        lines = NULL;
        lines_count = 0;
        int total = strlen(str);

        int len = total;
        while(*str) {
            len = strlen(str);

            bool canWrap = len > maxChars;
            while(len > maxChars) {
                len--;
            }

            if (canWrap && (len > 0)) {
                if ( isalpha(str[len - 1]) && isalpha(str[len]) ){
                    while((str[len - 1] != ' ')) {
                        len--;
                    }
                }
            }

            int i = lines_count++;
            /* lines = realloc(lines, sizeof(char *) * lines_count); */
            /* lines[i] = malloc(len + 1); */
            /* lines = realloc(lines, sizeof(char *) * lines_count); */
            MEMRES(lines, sizeof(char *) * lines_count);
            lines[i] = NULL;
            MEMRES(lines[i], len + 1);
            memcpy(lines[i], str, sizeof(char) * len);
            lines[i][len] = '\0';

            str += len;
        }

        for (int i = lines_count - 1; i >= 0; --i) {
            term->surface = \
                TTF_RenderText_Blended(term->ttf, lines[i], color);

            SDL_Rect r = _getTextRect(term, term->surface, scale, 0, j, true);

            bool isInTermWindow = r.y >= (term->window_size.h -
                              (term->height / scale));

            bool isOutSideOfCommads = r.y + r.h  <= (term->window_size.h -
                                      (term->surface->h / scale));

            if (isInTermWindow && isOutSideOfCommads) {
                term->texture = \
                    SDL_CreateTextureFromSurface(renderer, term->surface);

                SDL_RenderCopy(renderer, term->texture, NULL, &r);

                SDL_DestroyTexture(term->texture);
                term->texture = NULL;
            }
            /* free(lines[i]); */
            SDL_FreeSurface(term->surface);
            term->surface = NULL;
            j++;

        }
        for (int i = 0; i < lines_count; ++i) {
            free(lines[i]);
            lines[i] = NULL;
        }
        free(lines);
        lines = NULL;
        lines_count = 0;

    }
}

void terminal_free(Terminal *term)
{
    for (int i = 0; i < term->history_len; ++i) {
        free(term->history[i]);
    }
    free(term->history);
    for (int i = 0; i < term->messages_len; ++i) {
        free(term->messages[i].str);
    }
    for (int i = 0; i < lines_count; ++i) {
        free(lines[i]);
    }
    free(lines);
    free(term->messages);
    SDL_DestroyTexture(term->texture);
    SDL_FreeSurface(term->surface);
    TTF_CloseFont(term->ttf);
}

#endif // N2DE_TERMINAL_H_
