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
#ifndef N2DE_FILES_H_
#define N2DE_FILES_H_

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#define F_OK 0
#define access _access
#define MKDIR(path) CreateDirectoryA((LPCSTR)path, NULL);
#else
#include <sys/stat.h>
#define MKDIR(path) mkdir(path, 0700);
#include <unistd.h>
#endif

void getDataPath(char *path, const char *type, const uint8_t index)
{
    char str[50];
    memset(str, 0, 50);
    uint8_t num_width = floor(log10(index) + 1);
    snprintf(path, 
            strlen(type) + 1 + 
            strlen(str) + 1 + 
            num_width + 4, 
            "%s%s%u.dat", type, str, index);
}

#ifdef _WIN32
void removeDir(const char *path)
{
    char cmd[255];
    sprintf(cmd, "RMDIR /S /Q %s", path);
    system(cmd);
}
#else
void removeDir(const char *path)
{
    char cmd[255];
    sprintf(cmd, "rm -rf %s", path);
    system(cmd);
}
#endif

#ifdef _WIN32
void touch(const char *path)
{
    char cmd[255];
    sprintf(cmd, "copy /y nul \"%s\"", path);
    system(cmd);
}
#else
void touch(const char *path)
{
    char cmd[255];
    sprintf(cmd, "touch %s", path);
    system(cmd);
}
#endif

int fileno(FILE *stream);

uint32_t fileSize(FILE *fp)
{
    uint32_t size = 0;
    rewind(fp);
    fpos_t position;
    fgetpos(fp, &position);
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    fsetpos(fp, &position);
    return size;
}

long int fileToStr(FILE *fp, char *str)
{
    if ( !fp ){
        return 0;
    }

    rewind(fp);
    fpos_t position;
    fgetpos(fp, &position);

    fseek(fp,0, SEEK_END);
    long int size = ftell(fp);

    fsetpos(fp, &position);
    fread(str, size, 1, fp);
    str += size;
    *str++ = 0;

    return size;
}

#endif // N2DE_FILES_H_
