nyq.database.init("test.db", {
    biomes = {
        name = DB_TEXT_UNIQUE;
        map_id = DB_INTEGER_UNIQUE;
        effects = DB_TEXT_UNIQUE;
    },
    clonemon_types = {
        name = DB_TEXT_UNIQUE;
        ability = DB_TEXT;
        base_attack = DB_INTEGER;
        base_hp = DB_INTEGER;
        base_speed = DB_INTEGER;
        base_defense = DB_INTEGER;
        base_special_defense = DB_INTEGER;
        base_special_attack = DB_INTEGER;
        image = DB_TEXT;
    },
    move_types = {
        move_id = "moves";
        name = DB_TEXT_UNIQUE;
        base_power = DB_INTEGER;
        base_accuracy = DB_INTEGER;
        base_pp = DB_INTEGER;
        base_speed = DB_INTEGER;
        status_effect = DB_TEXT;
        image = DB_TEXT;
    },
    moves = {
        move_type_id = "move_types";
        pp = DB_INTEGER;
        power = DB_INTEGER;
        accuracy = DB_INTEGER;
        speed = DB_INTEGER;
    },
    move_types_compare = {
        move_types = "move_types";
        weak_id = "move_types";
        strong_id = "move_types";
        resistant_id = "move_types";
    },
    clonemon_biomes = {
        clonemon_id = "clonemon",
        biome_id = "biomes"
    },
    clonemon_moves = {
        clonemon_id = "clonemon";
        move_id = "moves"
    },
    clonemon = {
        type_id = "clonemon_types";
        name = DB_TEXT;
        gender = DB_INTEGER;
        level = DB_INTEGER;
        attack = DB_INTEGER;
        max_hp = DB_INTEGER;
        remaining_hp = DB_INTEGER;
        speed = DB_INTEGER;
        defense = DB_INTEGER;
        accuracy = DB_INTEGER;
        special_defense = DB_INTEGER;
        special_attack = DB_INTEGER;
        spritesheet = DB_TEXT_UNIQUE;
    },
    people = {
        age = DB_INTEGER;
        name = DB_TEXT;
        level = DB_INTEGER;
        spritesheet = DB_TEXT;
    },
    clonemon_party = {
        person_id = "people";
        clonemon = "clonemon";
    },
    players = {
        uuid = DB_TEXT_UNIQUE;
        person_id = "people";
    },
})
