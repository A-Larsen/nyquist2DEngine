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
#ifndef N2DE_FORMATS_H
#define N2DE_FORMATS_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../SDL2/SDL.h"
#include "../SDL2/SDL_ttf.h"

#include "declare.h"
#include "memory.h"
#include "config.h"
#include "files.h"


#define FORMATS_MAX_STRING 1024

static char **strings = NULL;
static uint16_t formats_count;
static uint8_t group_count = 0;

uint8_t formats_add(char *string)
{

    int i = formats_count++;
    MEMRES(strings, sizeof(char *) * formats_count);
    uint16_t len = strlen(string);
    strings[i] = NULL;
    MEMRES(strings[i], sizeof(char) * (len + 1));
    memcpy(strings[i], string, len);
    strings[i][len] = '\0';
    

    return i;
}

void formats_change()
{
}

void formats_clear()
{
    for (uint16_t i =0; i < formats_count; ++i) {
        free(strings[i]);
    }
    free(strings);
    strings = NULL;

}

uint16_t formats_newGroup()
{
    uint8_t i = group_count++;
    formats_clear();
    strings = NULL;
    formats_count = 0;
    return i;
}

bool formats_loadGroup(uint16_t id)
{
    char path[255];
    memset(path, 0, 255);
    sprintf(path, "./.cache/formats/%d", id);
    if (access(path, F_OK) != 0) {
        return false;
    }
    FILE *fp = fopen(path, "rb");

    uint16_t read = fread(&formats_count, sizeof(uint16_t), 1, fp);
    if (formats_count == 0) return false;
    if (read != 1) {
        N2DE_ERROR("could not read to group file\n");
    }
    MEMRES(strings, sizeof(char *) * formats_count);

    for (uint16_t i = 0; i < formats_count; ++i) {
        int len = 0;
        read = fread(&len, sizeof(uint16_t), 1, fp);
        strings[i] = NULL;
        MEMRES(strings[i], sizeof(char) * len);
        if (read != 1) {
            N2DE_ERROR("could not read to group file\n");
        }
        read = fread(strings[i], sizeof(char), len, fp);
        if (read != len) {
            N2DE_ERROR("could not read to group file\n");
        }
        
    }

    fclose(fp);
    return true;
}

void formats_saveGroup(uint16_t id)
{
    char path[255];
    memset(path, 0, 255);
    sprintf(path, "./.cache/formats/%d", id);
    FILE *fp = fopen(path, "wb+");

    uint16_t write = fwrite(&formats_count, sizeof(uint16_t), 1, fp);
    if (write != 1) {
        N2DE_ERROR("could not write to group file\n");
    }
    for (uint16_t i = 0; i < formats_count; ++i) {
        uint16_t len = strlen(strings[i]) + 1;
        write = fwrite(&len, sizeof(uint16_t), 1, fp);
        if (write != 1) {
            N2DE_ERROR("could not write to group file\n");
        }
        write = fwrite(strings[i], sizeof(char), len, fp);
        if (write != len) {
            N2DE_ERROR("could not write to group file\n");
        }
        
    }
    fclose(fp);
}

void formats_quit()
{
    formats_clear();
}

bool formats_getString(char string[FORMATS_MAX_STRING], uint16_t id)
{
    if (!strings) return false;
    memset(string, 0, FORMATS_MAX_STRING);
    memcpy(string, strings[id], strlen(strings[id]) * sizeof(char));
    return true;
}

#endif // N2DE_FORMATS_H
