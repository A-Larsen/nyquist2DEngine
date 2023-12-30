_ui = require("modules/ui_mt")
copy = require("modules/copy")
find = require("modules/find")
require("modules/string")
Collection = require("modules/collection")
State = require("modules/state")
require("modules/table")

require("./uis")
require("./states")
require("./database")


state_collection = Collection:new()
ui_collection = Collection:new()
params_collection = Collection:new()

function Push_state()
    state_collection:push(state)
    ui_collection:push(ui)
    params_collection:push(params)
end

function Pop_state()
    state = state_collection:pop()
    temps.ui = ui_collection:pop()
    params = params_collection:pop()
end

terminal = require("./terminal")

window_scale = 5
-- window_scale = 2

unscaled_window_size = {w = 240, h = 160}
window_size = {w = unscaled_window_size.w * window_scale, h = unscaled_window_size.h * window_scale}
params = {}
-- controller = "keyboard"
controller = "gamepad"
frame_rate = 60


nyq.init(
    "clonemon",
    {x = 400, y = 100},
    window_size,
    {
        frame_rate = frame_rate,
        controlls = {
            [1] = {
                move_north =  {key = "dpup", trigger = false, keyrepeat = false},
                move_east =  {key = "dpright", trigger = false, keyrepeat = false},
                move_south =  {key = "dpdown", trigger = false, keyrepeat = false},
                move_west = {key = "dpleft", trigger = false, keyrepeat = false},
                move_up =  {key = "dpup", trigger = true, keyrepeat = false},
                move_right =  {key = "dpright", trigger = true, keyrepeat = false},
                move_down =  {key = "dpdown", trigger = true, keyrepeat = false},
                move_left = {key = "dpleft", trigger = true, keyrepeat = false},
                change_moves1 = {key = "I", trigger = false, keyrepeat = false},
                change_moves2 = {key = "O", trigger = false, keyrepeat = false},
                select = {key = "a", trigger = false, keyrepeat = false},
                back = {key = "b", trigger = true, keyrepeat = false},
                bag = {key = "y", trigger = true, keyrepeat = false},
                pause = {key = "start", trigger = true, keyrepeat = false},
                hotkey = {key = "x", trigger = true, keyrepeat = false},
            }
            -- [1] = {
            --     move_north =  {key = "W", trigger = false, keyrepeat = true},
            --     move_east =  {key = "D", trigger = false, keyrepeat = true},
            --     move_south =  {key = "S", trigger = false, keyrepeat = true},
            --     move_west = {key = "A", trigger = false, keyrepeat = true},
            --     move_up =  {key = "K", trigger = true, keyrepeat = true},
            --     move_right =  {key = "L", trigger = true, keyrepeat = true},
            --     move_down =  {key = "J", trigger = true, keyrepeat = true},
            --     move_left = {key = "H", trigger = true, keyrepeat = true},
            --     change_moves1 = {key = "I", trigger = true, keyrepeat = false},
            --     change_moves2 = {key = "O", trigger = true, keyrepeat = false},
            --     select = {key = "Return", trigger = true, keyrepeat = false},
            --     pause = {key = "Escape", trigger = true, keyrepeat = false},
            -- }
        },
        fullscreen = false,
        assign_controllers = {
            -- [1] = {controller = "gamepad", id = 0},
            [1] = {controller = controller, id = 0},
            -- [2] = {controller = "keyboard", id = 1},
            -- [3] = {controller = "keyboard", id = 1},
            -- [4] = {controller = "keyboard", id = 1},
        }
    }
)

nyq.audio.init({
    intro = {file = "intro.wav", loop = -1},
    world = {file = "pianer1.wav", loop =  -1},
    battle = {file = "battle1.wav", loop = -1},
})

uis.start:init("logo.png", unscaled_window_size, {scale = window_scale})
uis.world:init("nyquist_spritesheet.png", "test", "tileset_indoors_1.png", unscaled_window_size, {scale = window_scale})
uis.battle:init("clonemon_battle_ui.png", unscaled_window_size, {scale = window_scale})
uis.loadsave:init("loadsave.png", unscaled_window_size, {scale = window_scale})
uis.enter_text:init(unscaled_window_size, {scale = window_scale})
uis.choice_select:init(unscaled_window_size, {scale = window_scale})
uis.bag:init(unscaled_window_size, {scale = window_scale})
uis.party:init(unscaled_window_size, {scale = window_scale})

selected = 1

temps = {
    ui = nil
}
moves = {
    charzard = {
        [1] = {name = "kick", pp = {remaining = 12, total = 50}, type = "fire"},
        [2] = {name= "punch", pp = {remaining = 15, total = 20}, type = "physical"},
        [3] = {name= "slash", pp = {remaining = 15, total = 40}, type = "metal"},
        [4] = {name= "fly", pp = {remaining = 15, total = 10}, type = "flying"},
    },
    pikachu = {
        [1] = {name = "sleep", pp = {remaining = 12, total = 50},

               type = "psychic"},
        [2] = {name= "static", pp = {remaining = 15, total = 20},
               type = "electric"},
        [3] = {name= "jump", pp = {remaining = 15, total = 40},
               type = "flying"},
        [4] = {name= "kick", pp = {remaining = 15, total = 10},
                   type = "physical"},
    }
}

my_clonemon = {
    [1] = {
        name = "Charzard",
        moves = moves.charzard,
        max_hp = 100,
        hp = 50,
        level = 100,
    },
    [2] = {
        name = "Pikachu",
        moves = moves.charzard,
        max_hp = 50,
        hp = 10,
        level = 30,
    },
    [3] = {
        name = "um",
        moves = moves.charzard,
        max_hp = 50,
        hp = 10,
        level = 30,
    },
    [4] = {
        name = "Meowth",
        moves = moves.charzard,
        max_hp = 90,
        hp = 50,
        level = 30,
    },
    [5] = {
        name = "Thing",
        moves = moves.charzard,
        max_hp = 90,
        hp = 50,
        level = 30,
    },
    [6] = {
        name = "not thing",
        moves = moves.charzard,
        max_hp = 90,
        hp = 50,
        level = 30,
    },
}


temps.ui = uis.start
--
local pos = {x = 177, y = 1745}
-- uis.world:setPlayerPosition({x = pos.x * 2 , y = -(pos.y *  2)})
-- uis.world:setPlayerPosition({x = 177 , y = 1745})
-- uis.world:setPlayerPosition({x = 400 , y = 240})
uis.world:setPlayerPosition({x = 640 , y = 0})
state = states.start.main

local padding = 11
local padding_top_and_bottom = 8
nyq.loops.init()
local start = true

local loop_id = nyq.loops.add(60)
local test_text = [[hello there! this is just a lot of text just for the heck of it I hope it doesn't annoy you at all, it is just what happens when you have a lot of text. Isn't this game really cool! Can't believe that everything seems to be working so smoothly. What is your name? oh cool. that is a nice name. hope that you can continue your adventure. What in the world is this? That is not normal though. ahahahah.]]
while not nyq.quit do
    nyq.loops.loopStart(loop_id)
    -- you cannot start the game with a state that has controls

    if temps.ui then

        if ui and ui.free then
            ui:free()
        end

        temps.ui:start(params)

        ui = temps.ui

        temps.ui = nil
    end
    if state then
        state()
    end
    if not start then
        nyq.controlsUpdate()
    end
    nyq.clear({r = 0, g = 0, b = 0, a = 255})
    if ui == uis.battle then
            uis.battle:updateBackground()
            uis.battle:updateMyHealth()

        if (state == states.battle.enterBattle) then
            uis.battle:updateNarration("enter_battle")
            -- uis.battle:updateNarration("custom", test_text)
            -- uis.battle:updateDo(selected)
        elseif (state == states.battle.doSelect) then
            uis.battle:updateDo(selected)
        elseif (state == states.battle.moveSelect) then
            uis.battle:updateMoveSelect(selected)
        end

    elseif ui == uis.world then
        uis.world:worldUpdate()


        if state == states.world.option then
            uis.world:textUpdate(test_text)
            uis.world:optionSelectUpdate({"yes", "no"})
            -- uis.world:textYesNoUpdate()

        end
        if state == states.world.pause then
            uis.world:pauseUpdate()
        elseif state == states.world.text then
            uis.world:textUpdate(test_text)
        end

    elseif ui == uis.start then
        start = uis.start:update()
    elseif ui == uis.loadsave then
        uis.loadsave:update()
    elseif ui == uis.enter_text then
        uis.enter_text:update()
    elseif ui == uis.choice_select then
        uis.choice_select:update()
    elseif ui == uis.bag then
        uis.bag:updateBag()
    elseif ui == uis.party then
        if state == states.party.main then
            ui:partyUpdate()
        elseif state == states.party.doWhat then
            ui:doWhatUpdate()
        elseif state == states.party.switch then
            ui:switchUpdate()
        end
    end

    nyq.draw()
    nyq.loops.loopEnd(loop_id)
 end
