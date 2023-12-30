spritesheet = "tileset_indoors_1.png"
world_name = "test"
-- background_image = "outdoors_backgorund_one.png"
-- background_image = "outdoors_backgorund_two.png"
background_image = "Route_111.png"
-- background_image = "battle_ui.png"

keys = {
    zoom_in = "I",
    zoom_out = "O",
    move_left = "A",
    move_right = "D",
    move_up = "W",
    move_down = "S",
    save = "LCTRL S",
    toggle_info = "C",
    add_object = "LALT O",
    toggle_object_select = "LSHIFT Tab",
    next = "N",
    previous = "P",
    -- any popup can be toggled to fullscreen
    delete = "Delete",
    toggle_fullscreen = "LSHIFT Return",
    position_reset = "R",
    mode_place = "1",
    mode_select = "2",
    mode_move = "3"
}

map = {
    grid = 8,
    -- width = 340 * 5,
    -- height = 160 * 5
    width = 640 * 2,
    height = 2240 * 2
}

objects = {

    floor = {
        crops = {
            [1] = {x = 8, y = 0, w = 8, h = 8},
            [2] = {x = 8, y = 8, w = 8, h = 8},
        },
        -- z and y of hit box are reltitive to crop x and y
        hitbox = {x = 0, y = 0, w = 8, h = 8},
        is_collidable = false,
        is_visable = true,
    },

    stairs = {
        crops = {
            [1] = {x = 8, y = 16, w = 16, h = 16},
        },
        hitbox = {x = 0, y = 0, w = 16, h = 16},
        is_collidable = false,
        is_visable = true
    },

    tv = {
        crops = {
            [1] = {x = 56, y = 32, w = 16, h = 32},
        },
        hitbox = {x = 0, y = 23, w = 16, h = 8},
        is_collidable = true,
        is_visable = true
    },

    table = {
        crops = {
            [1] = {x = 72, y = 160, w = 32, h = 32},
        },
        hitbox = {x = 0, y = 0, w = 32, h = 32},
        is_collidable = true,
        is_visable = true
    },

    tree = {
        crops = {
            [1] = {x = 112, y = 184, w = 16, h = 32},
        },
        hitbox = {x = 0, y = 21, w = 16, h = 10},
        is_collidable = true,
        is_visable = true
    },
    bookshelf = {
        crops = {
            [1] = {x = 72, y = 32, w = 32, h = 32},
        },
        hitbox = {x = 0, y = 22, w = 32, h = 8},
        is_collidable = true,
        is_visable = true
    },

    armoire = {
        crops = {
            [1] = {x = 32, y = 32, w = 24, h = 32},
        },
        hitbox = {x = 0, y = 18, w = 24, h = 14},
        is_collidable = true,
        is_visable = true
    },

    sink = {
        crops = {
            [1] = {x = 0, y = 64, w = 32, h = 24},
        },
        hitbox = {x = 0, y = 12, w = 32, h = 11},
        is_collidable = true,
        is_visable = true
    },

    nightstand = {
        crops = {
            [1] = {x = 104, y = 32, w = 16, h = 24},
        },
        hitbox = {x = 0, y = 14, w = 16, h = 10},
        is_collidable = true,
        is_visable = true
    },

    rug = {
        crops = {
            [1] = {x = 120, y = 28, w = 8, h = 16},
        },
        hitbox = {x = 0, y = 0, w = 8, h = 16},
        is_collidable = false,
        is_visable = true
    },

    statue = {
        crops = {
            [1] = {x = 0, y = 128, w = 16, h = 32},
        },
        hitbox = {x = 0, y = 9, w = 16, h = 23},
        is_collidable = true,
        is_visable = true
    },

    footrest = {
        crops = {
            [1] = {x = 65, y = 192, w = 16, h = 16},
        },
        hitbox = {x = 0, y = 0, w = 16, h = 16},
        is_collidable = false,
        is_visable = true
    },

    picture = {
        crops = {
            [1] = {x = 36, y = 193, w = 24, h = 16},
        },
        hitbox = {x = 0, y = 0, w = 24, h = 16},
        is_collidable = false,
        is_visable = true
    },

    cash_register = {
        crops = {
            [1] = {x = 112, y = 128, w = 16, h = 24},
        },
        hitbox = {x = 2, y = 8, w = 14, h = 12},
        is_collidable = true,
        is_visable = true
    },

    window = {
        crops = {
            [1] = {x = 56, y = 88, w = 16, h = 16},
        },
        hitbox = {x = 0, y = 0, w = 16, h = 16},
        is_collidable = false,
        is_visable = true
    },

    couch = {
        crops = {
            [1] = {x = 16, y = 192, w = 16, h = 32},
        },
        hitbox = {x = 0, y = 10, w = 16, h = 22},
        is_collidable = false,
        is_visable = false
    },

    terminal = {
        crops = {
            [1] = {x = 104, y = 152, w = 16, h = 32},
        },
        hitbox = {x = 0, y = 20, w = 16, h = 12},
        is_collidable = true,
        is_visable = true
    },

}
