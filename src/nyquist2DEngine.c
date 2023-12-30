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
#define _GNU_SOURCE

#include "N2DE/N2DE.h"

int main(int args, char **argv)
{
    int argi = engine_handleOptions(args, argv);


    if (argv[argi] == NULL) {
        return 0;
    }

    Nyquist2DEngine engine;
    if (argi == -1) {
        engine_init(&engine, "./init.lua");
    } else {
        engine_init(&engine, argv[argi]);
    }

    engine_run(&engine);

    engine_destroy(&engine);
    return 0;
}
