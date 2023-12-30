local ui = {}

function ui:init(window_size, options)
    setmetatable(ui, _ui)
    ui:_init(nil, window_size, "NotoSansMono-Regular.ttf", 6, options, function()
        ui.selected = 1
    end)
end

function ui:startCallback()
    ui.notSelected_rects = {}

    local fg_color = {r = 255, g = 255, b = 255, a = 160}
    local fg_color2 = {r = 0, g = 0, b = 0, a = 255}
    local bg_color = {r = 0, g = 0, b = 0, a = 0}

    ui.narration = {}
    ui:addText("narration", "choice", "Choose a Clonemon", 4, fg_color2, bg_color)
    ui:addText("narration", "back", "go back", 4, fg_color2, bg_color)
    ui:addText("narration", "do_what", " ", 6, fg_color2, bg_color)

    for i = 1, 7 do
        ui.notSelected_rects[i] = {}
    end

    ui.notSelected_rects[1].position = {x = 3, y = 4}
    ui.notSelected_rects[2].position = {x = 121, y = 4}
    ui.notSelected_rects[3].position = {x = 3, y = 50}
    ui.notSelected_rects[4].position = {x = 121, y = 50}
    ui.notSelected_rects[5].position = {x = 3, y = 96}
    ui.notSelected_rects[6].position = {x = 121, y = 96}
    ui.notSelected_rects[7].position = {x = 121, y = 143}
    ui.clonemon_switch = 0

    ui.clonemon = {}

    for i, clonemon in pairs(params.my_clonemon) do
        ui.clonemon[i] = {}
        ui.clonemon[i].ids = {}
        ui:addText2(ui.clonemon[i].ids, "name", clonemon.name, 4, fg_color, bg_color)
        ui:addText2(ui.clonemon[i].ids, "max_hp", clonemon.max_hp, 4, fg_color, bg_color)
        ui:addText2(ui.clonemon[i].ids, "hp", clonemon.hp, 4, fg_color, bg_color)
        ui:addText2(ui.clonemon[i].ids, "level", clonemon.level, 4, fg_color, bg_color)
    end

    for i = 1, 7 do
        ui.notSelected_rects[i].id = ui:createRect({r = 0, g = 0, b = 0, a = 125}, true, 3)
    end

    local rect_color = {r = 255, g = 255, b = 255, a = 255}

    ui.narration.rect = {
        id = ui:createRect(rect_color, true, 3),
        position = {x = 3, y = 143},
        size = {w = 116, h = 15}
    }

    ui.goBack = {
        id = ui:createRect({r = 102, g = 114, b = 146, a = 255}, true, 3),
        position = {x = 121, y = 143},
        size = {w = 116, h = 15}
    }

    ui.doWhat = {
        id = ui:createRect(rect_color, true, 5),
        position = {x = 121, y = 50},
        size = {w = 116, h = 108}
    }

    ui.boxes_id = ui:imageCreate("clonemon_party.png", 1)
    ui.clonemon_selected = 1
end

function ui:partyUpdate()
    ui:imageUpdate(ui.boxes_id, {x = 0, y = 0}, {w = ui.window_size.w, h = 140})
    ui:rectUpdate(ui.narration.rect.id, ui.narration.rect.position, ui.narration.rect.size)
    ui:rectUpdate(ui.goBack.id, ui.goBack.position, ui.goBack.size)
    ui:updateText("narration", "choice", {x = 6, y = 146})
    ui:updateText("narration", "back", {x = 200, y = 146})
    if params.my_clonemon then
        for i, clonemon in pairs(params.my_clonemon) do
            local name_pos = {
                x = ui.notSelected_rects[i].position.x + 4,
                y = ui.notSelected_rects[i].position.y + 4,
            }
            ui:updateText2(ui.clonemon[i].ids, "name", name_pos)
        end
    end
    for i = 1, 6 do
        if ui.selected ~= i then
            ui:rectUpdate(ui.notSelected_rects[i].id, ui.notSelected_rects[i].position, {w = 116, h = 44})
        end
    end
    if ui.selected ~= 7 then
        ui:rectUpdate(ui.notSelected_rects[7].id, ui.notSelected_rects[7].position, {w = 116, h = 15})
    end
end

function ui:doWhatUpdate()
    local fg_color2 = {r = 0, g = 0, b = 0, a = 255}
    local bg_color = {r = 0, g = 0, b = 0, a = 0}
    ui:imageUpdate(ui.boxes_id, {x = 0, y = 0}, {w = ui.window_size.w, h = 140})
    ui:rectUpdate(ui.narration.rect.id, ui.narration.rect.position, ui.narration.rect.size)
    -- ui:rectUpdate(ui.doWhat.id, ui.doWhat.position, ui.doWhat.size)
    --
    self:optionSelect({"Summary", "Switch", "Item", "Cancel"},
        {
        x = ui.notSelected_rects[ui.clonemon_selected].position.x + 116 - 8,
        y = ui.notSelected_rects[ui.clonemon_selected].position.y + 22,
    }
    , ui.selected)
    local format =  string.format("Do what with %s?", params.my_clonemon[ui.clonemon_selected].name)

    ui:textChange("narration", "do_what", format , fg_color2, bg_color)
    ui:updateText("narration", "do_what", {x = 6, y = 146})

    if params.my_clonemon then
        for i, clonemon in pairs(params.my_clonemon) do
            local name_pos = {
                x = ui.notSelected_rects[i].position.x + 4,
                y = ui.notSelected_rects[i].position.y + 4,
            }
            ui:updateText2(ui.clonemon[i].ids, "name", name_pos)
        end
    end
    for i = 1, 6 do
        if ui.clonemon_selected ~= i then
            ui:rectUpdate(ui.notSelected_rects[i].id, ui.notSelected_rects[i].position, {w = 116, h = 44})
        end
    end
    if ui.clonemon_selected ~= 7 then
        ui:rectUpdate(ui.notSelected_rects[7].id, ui.notSelected_rects[7].position, {w = 116, h = 15})
    end
end

function ui:switchUpdate()
    local fg_color2 = {r = 0, g = 0, b = 0, a = 255}
    local bg_color = {r = 0, g = 0, b = 0, a = 0}
    ui:imageUpdate(ui.boxes_id, {x = 0, y = 0}, {w = ui.window_size.w, h = 140})
    ui:rectUpdate(ui.narration.rect.id, ui.narration.rect.position, ui.narration.rect.size)
    -- ui:rectUpdate(ui.doWhat.id, ui.doWhat.position, ui.doWhat.size)
    --
    -- local format =  string.format("Do what with %s?", params.my_clonemon[ui.clonemon_selected].name)

    ui:textChange("narration", "do_what", "Move to where?" , fg_color2, bg_color)
    ui:updateText("narration", "do_what", {x = 6, y = 146})

    if params.my_clonemon then
        for i, clonemon in pairs(params.my_clonemon) do
            local name_pos = {
                x = ui.notSelected_rects[i].position.x + 4,
                y = ui.notSelected_rects[i].position.y + 4,
            }
            ui:updateText2(ui.clonemon[i].ids, "name", name_pos)
        end
    end
    for i = 1, 6 do
        if ui.clonemon_selected ~= i  and ui.clonemon_switch ~= i then
            ui:rectUpdate(ui.notSelected_rects[i].id, ui.notSelected_rects[i].position, {w = 116, h = 44})
        end
    end
    if ui.clonemon_selected ~= 7 then
        ui:rectUpdate(ui.notSelected_rects[7].id, ui.notSelected_rects[7].position, {w = 116, h = 15})
    end
end

return ui
