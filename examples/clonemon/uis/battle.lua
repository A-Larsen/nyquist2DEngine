local ui = {}

function ui:init(spritesheet, window_size, options)
    setmetatable(ui, _ui)
    ui:_init(spritesheet, window_size, "NotoSansMono-Regular.ttf", 8, options, function()
        ui.narration = {}
        ui.narration.fg_color = {r = 255, g = 255, b = 255, a = 255}
        ui.narration.bg_color = {r = 255, g = 255, b = 255, a = 0}
        -- ui.narration.id = nil
        -- ui.narration.text = {}
        -- ui.narration.page_idx = 0
        -- ui.narration.pages = 1
        -- ui.narration.text.custom = " "
        -- ui.narration.string = nil

        ui.background.position = {x = 0, y = 0}
        ui.background.size = window_size


        ui.current_pokemon = ""
        ui.move_select = {}
        ui.move_select.moves = {}
        ui.move_select.pp = {}
        ui.move_select.pp.position = {x = 170, y = 133}
        ui.move_select.type = {}
        ui.move_select.type.position = {x = 170, y = 145}
        ui.line_count = 2
        ui.line_ids = {}

        local positions = {
            [1] = {x = 12, y = 133},
            [2] = {x = 82, y = 133},
            [3] = {x = 12, y = 145},
            [4] = {x = 82, y = 145}
        }

        for i = 1, #positions do
            ui.move_select.moves[i] = {}
            ui.move_select.moves[i].string = nil
            ui.move_select.moves[i].position = positions[i]
            ui.move_select.moves[i].pp_id = nil
            ui.move_select.moves[i].name_id = nil
            ui.move_select.moves[i].type_id = nil
        end
        ui.text_box = {}
        ui.text_box.position = {x = 0, y = 130}
        ui.triggered_song = false


        ui.formats = {}
        ui.formats.pokemon_enter_id = nyq.formats.create("A Wild Pokemon has appeared")
        ui.formats.enter_battle_id = nyq.formats.create("%s entered the battle")
        ui.formats.lose_id = nyq.formats.create("you lost")
        ui.formats.lose_money_id = nyq.formats.create("you get %d for winning")
        ui.formats.do_id = nyq.formats.create("what will %s do?")
    end)

end


function ui:startCallback()
    nyq.hasFrameCap(true)
    ui.menu_items_id = ui:newSprite("menu_items.png")
    ui:linesCreate("narration", 3, ui.narration.fg_color, ui.narration.bg_color)
    ui:spriteAddCrops(ui.menu_items_id, "selection arrow", 6, {
        {
            x = 7,
            y = 0,
            w = 7,
            h = 7
        }
    })
    ui:addCrop("my_health",
        {x = 0, y = 30, w = 100, h = 28},
        {w = 100, h = 28}
    )
    ui:addCrop("do",
        {x = 100, y = 28, w = 85, h = 30},
        {w = 85, h = 30}
    )

    ui.narration.background_rect_id = ui:createRect({r = 0, g = 0, b = 0, a = 200}, true, 4)
    ui:addCrop("move_select",
        {x = 0, y = 0, w = 240, h = 30},
        {w = 240, h = 30}
    )
    ui:addBackgroundImage("grassland", "clonemon_battle_grass.png")
    ui:setBackgroundImage("grassland")

    local format = nyq.formats.getString(ui.formats.enter_battle_id)
    local str = string.format(format, ui.params.rivals_pokemon[1].name)

    ui:addText("narration", "enter_battle", str, 5, ui.narration.fg_color, ui.narration.bg_color)

    local fg_color = {r = 0, g = 0, b = 0, a = 255}
    local bg_color = {r = 0, g = 0, b = 0, a = 0}
    ui:addText("pokemon", "name", params.my_pokemon.name, 5, fg_color, bg_color)
    ui:addText("pokemon", "level", params.my_pokemon.level, 5, {r = 0, g = 0, b = 0, a = 255},
                                            {r = 0, g = 0, b = 0, a = 0})
    format = nyq.formats.getString(ui.formats.do_id)
    str = string.format(format, ui.params.player.name)

    ui:addText("do_narration", "narration", str, 5, ui.narration.fg_color,
                                            ui.narration.bg_color)

    ui.do_options = {}
    for i = 1, 4 do
        ui.do_options[i] = {}
    end
    ui.do_options[1].position = {x = 167, y = 132}
    ui.do_options[2].position = {x = 220, y = 132}
    ui.do_options[3].position = {x = 167, y = 132 + ui.font_size + 1}
    ui.do_options[4].position = {x = 220, y = 132 + ui.font_size + 1}
    ui:addText2(ui.do_options[1], "id", "fight", 6, fg_color, bg_color)
    ui:addText2(ui.do_options[2], "id", "bag", 6, fg_color, bg_color)
    ui:addText2(ui.do_options[3], "id", "clonemon", 6, fg_color, bg_color)
    ui:addText2(ui.do_options[4], "id", "run", 6, fg_color, bg_color)

    local fg_color = {r = 0, g = 0, b = 0, a = 0}
    local bg_color = {r = 255, g = 255, b = 255, a = 0}

    for i, move in ipairs(ui.move_select.moves) do
        ui:addText2(move, "name_id", ui.params.my_pokemon.moves[i].name, 5, fg_color, bg_color)

        str = string.format("pp %s/%s", ui.params.my_pokemon.moves[i].pp.remaining,
                            ui.params.my_pokemon.moves[i].pp.total)

        ui:addText2(move, "pp_id", str, 5, fg_color, bg_color)

        str = string.format("%s", ui.params.my_pokemon.moves[i].type)

        ui:addText2(move, "type_id", str, 5, fg_color, bg_color)
    end
    nyq.audio.stopAll()
    nyq.audio.play("battle")
end

function ui:updateMyHealth()
    ui:updateCrop("my_health", {x = 139, y = 90})
    ui:updateText("pokemon", "name", {x = 140, y = 90})
    ui:updateText("pokemon", "level", {x = 223, y = 90})
end


function ui:updateNarration(type, text)
    ui:rectUpdate(ui.narration.background_rect_id, ui.text_box.position, {w = 240, h = 30})

    local position = copy.shallow_copy(ui.text_box.position)
    position.x = position.x + 4
    position.y = position.y + 0

    -- need worldwrap
    if type == "enter_battle" then
        ui.narration.pages = 1
        ui:updateText("narration", "enter_battle", position)
    elseif type == "custom" then
        ui:linesDraw("narration", text, 48, position)
    end
end

function ui:updateDo(selected)
    ui:rectUpdate(ui.narration.background_rect_id, ui.text_box.position, {w = 155, h = 30})
    ui:updateCrop("do", {x = 155, y = 130})
    ui:updateText("do_narration", "narration", {x = 4, y = 130})

    for i = 1, 4 do
        ui:updateText2(ui.do_options[i], "id", ui.do_options[i].position)
    end

    local size = ui:getTextSize2(ui.do_options[selected], "id")
    local text_pos = ui.do_options[selected].position

    ui:spriteUpdateCrop(ui.menu_items_id, "selection arrow", 1, {
        x = text_pos.x - 8 , y = text_pos.y + 2
    })
end

function ui:addMoves(moves)
    local fg_color = {r = 0, g = 0, b = 0, a = 0}
    local bg_color = {r = 255, g = 255, b = 255, a = 0}

    for i, move in ipairs(ui.move_select.moves) do
        str = string.format("pp %s/%s", moves[i].pp.remaining,
                            moves[i].pp.total)
        str = string.format("%s", moves[i].type)
    end
end

function ui:updateMoveSelect(selected)

    ui:updateCrop("move_select", ui.text_box.position)

    for i, move in ipairs(ui.move_select.moves) do
        ui:updateText2(move, "name_id", move.position)
    end

    move = ui.move_select.moves[selected]
    ui:updateText2(move, "pp_id", ui.move_select.pp.position)
    ui:updateText2(move, "type_id", ui.move_select.type.position)

    local size = ui:getTextSize2(ui.move_select.moves[selected], "name_id")
    local text_pos = ui.move_select.moves[selected].position

    ui:spriteUpdateCrop(ui.menu_items_id, "selection arrow", 1, {
        x = text_pos.x - 8 , y = text_pos.y + 2
    })

end

return ui
