local ui = {}

function ui:init(window_size, options)
    setmetatable(ui, _ui)
    ui:_init(nil, window_size, "NotoSansMono-Regular.ttf", 8, options, function()
        ui.string_id = {}
        ui.line_count = 5
        ui.character_length = 46
        ui.string = ""
        ui.full = false
        ui. rect_position = {y = 0, x = 0}
        ui.input = {}
        ui.character_limit = nil
        ui.clonemon_image_id = nil
        ui.title = nil
    end)
end


function ui:startCallback()
    nyq.hasFrameCap(false)
    ui:addBackgroundImage("background", "clonemon_battle_grass.png")
    ui:setBackgroundImage("background")
    ui.rect_id = ui:createRect({r = 0, g = 0, b = 0, a = 180}, true, 0)
    ui.blinker = ui:createRect({r = 255, g = 255, b = 255, a = 255}, true, 0)
    ui.clonemon_image_id = ui:newSprite(params.sprite)
    ui.string = ""
    ui:spriteAddCrops(ui.clonemon_image_id, "clonemon", 0, {
        {x = 105, y = 0, w = 91 , h = 62}
    })
    local fg_color = {r = 0, g = 0, b = 0, a = 0}
    local bg_color = {r = 255, g = 255, b = 255, a = 0}
    ui.character_limit = params.character_limit
    ui.title = params.title
    ui.cursor = 0
    if ui.title then
        ui:addText("text", "input", ui.title, 5, {r = 0, g = 0, b = 0, a = 0},
        {r = 0, g = 0, b = 0, a = 0})
    end
    ui.line_count = 1

    if (controller == "keyboard") then
        ui.rect_position.x = 0
        ui.rect_position.y = 115
    elseif (controller == "gamepad") then
        ui.rect_position.x = 0
        ui.rect_position.y = 80
        ui.keyboard_upper_id = ui:imageCreate("keyboard_upper.png", 1)
        ui.keyboard_lower_id = ui:imageCreate("keyboard_lower.png", 1)
        ui.key_selector_id = ui:createRect({r = 255, g = 255, b = 255, a = 100}, true, 2)
        ui.keyboard_background__id = ui:createRect({r = 51, g = 51, b = 51, a = 200}, true, 0)
        ui.isUpper = false
        ui.key_change_case = "\n"
        ui.key_backspace = "\b"
        ui.key_done = "\z"
        ui.letter_selection = {
            "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", " ",
            "a", "s", "d", "f", "g", "h", "j", "k", "l", " ", ui.key_backspace,
            "z", "x", "c", "v", "b", "n", "m", " ",ui.key_change_case, ui.key_done, ui.key_done,
        }
    end
        ui.rect_size = {w = 91 , h = 11}

    ui:addText2(
        ui.string_id,
        "line",
        "Enter Name", -- blank string here causes crashing
        5,
        {r = 0, g = 0, b = 0, a = 255},
        {r = 0, g = 0, b = 0, a = 0}
    )
end

function ui:update()
    ui:updateBackground()
    local title_offset = 0
    local text_offset = 0
    local title_size = nil
    if ui.title then
        title_size = ui:getTextSize("text", "input")
        text_offset = (ui.window_size.w / 2) - (((ui.rect_size.w) - (title_size.w)) / 2)
        title_offset = text_offset - (title_size.w)
        ui:updateText("text", "input", { x = title_offset, y = ui.rect_position.y})
    end

    ui:rectUpdate(ui.rect_id, {x = ui.rect_position.x + (text_offset), y = ui.rect_position.y},
                              ui.rect_size)

    local position = nil
    local y = ((ui.rect_position.y) - (1))
    if (controller == "keyboard") then
        position = {
            x = ui.window_size.w / 2 - (91 / 2),
            y = 30
        }
    elseif (controller == "gamepad") then
        local cx = ui.cursor % 11
        local cy = math.floor(ui.cursor / 11)
        if ui.isUpper then
            ui:imageUpdate(ui.keyboard_upper_id, {x = 4, y = 98}, {w = 230, h = 59})
        else
            ui:imageUpdate(ui.keyboard_lower_id, {x = 4, y = 98}, {w = 230, h = 59})
        end
        local w = 23
        if ui.letter_selection[ui.cursor + 1] == ui.key_done then
            w = 44
            if ui.cursor == 32 then
                cx = cx - 1
            end
        end
        ui:rectUpdate(ui.key_selector_id, {x = 3 + (21 * cx), y = 97 + (20 * cy)},
                            {w = w , h = 21})
        ui:rectUpdate(ui.keyboard_background__id, {x = 3, y = 97},
                            {w = 233 , h = 61})
        position = {
            x = ui.window_size.w / 2 - (91 / 2),
            y = 9
        }
    end

    ui:spriteUpdateCrop(ui.clonemon_image_id, "clonemon", 1, position)

    if string.len(ui.string) == 0 then
        if nyq.getTicks() % 600 <= 300 then
            ui:rectUpdate(ui.blinker, {x =  3 + text_offset + ui.rect_position.x, y = y + 2},
                                      {w = (ui.font_size) / 4, h = (ui.font_size )})
        end
        return
    end


    local strlen = string.len(ui.string)
    if strlen >= ui.character_length then
        ui.full = true
    end

    if not (strlen > 0) then return end

    ui:textChange2(
        ui.string_id,
        "line",
        ui.string,
        {r = 255, g = 255, b = 255, a = 255},
        {r = 0, g = 0, b = 0, a = 0}
    )

    local x = 3 + text_offset + ui.rect_position.x
    ui:updateText2(ui.string_id, "line",
        {x = x, y = y }
    )
    local size = ui:getTextSize2(ui.string_id, "line")

    if not ui.full then
        local position = {x = x + size.w, y = y + 2}
        local size = {w = (ui.font_size) / 4, h = (ui.font_size)}
        if nyq.getTicks() % 600 <= 300 then
            if strlen < ui.character_length then
                ui:rectUpdate(ui.blinker, position, size)

            elseif(i + 1 <= ui.line_count) then
                ui:rectUpdate(ui.blinker, position, size)
            end
        end
    end


end

return ui
