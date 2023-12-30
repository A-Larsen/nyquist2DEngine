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
#ifndef N2DE_ERROR_H_
#define N2DE_ERROR_H_

#define _GNU_SOURCE

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#define N2DE_ERROR(...) \
    _error(__FILE__, __FUNCTION__, __LINE__,  __VA_ARGS__);

void _error(const char *file, const char *function, uint16_t line, const char *fmt,...){
	va_list args;
	va_start(args, fmt);
    char newstr[500];
	vsnprintf(newstr, 500, (const char *)fmt, args);

    fprintf(stderr, 
            "\n"
			"\x1b[91mERROR \x1b[39m\n\n"
            "\x1b[92mfile\x1b[39m         %s\n"
            "\x1b[92mfunction\x1b[39m     %s\n"
            "\x1b[92mline\x1b[39m         %d\n"
            "\n%s\n\n",
            file, function,
            line, newstr);


	va_end(args);
}

#endif // N2DE_ERROR_H_
