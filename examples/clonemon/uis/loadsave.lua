local ui = {}

function ui:init(spritesheet, window_size, options)
    setmetatable(ui, _ui)
    ui:_init(spritesheet, window_size, "NotoSansMono-Regular.ttf", 8, options, function()
        ui.players = {}
        ui.selected = 1
        ui.selected2 = 1
        ui.menu_size = 2
    end)
end

function ui:startCallback()
    local fg_color = {r = 0, g = 0, b = 0, a = 0}
    local bg_color = {r = 255, g = 255, b = 255, a = 0}
    ui:addCrop("load_selected",
        {x = 0, y = 0, w = 208, h = 80},
        {w = 208, h = 80}
    )
    ui:addCrop("box1_unselected",
        {x = 208, y = 0, w = 208, h = 80},
        {w = 208, h = 80}
    )

    ui:addCrop("box2_selected",
        {x = 0, y = 80, w = 208, h = 32},
        {w = 208, h = 32}
    )
    ui:addCrop("box2_unselected",
        {x = 208, y = 80, w = 208, h = 32},
        {w = 208, h = 32}
    )
    ui:addCrop("right_arrow",
        {x = 416, y = 0, w = 22, h = 27},
        {w = 22, h = 27}
    )
    ui:addCrop("left_arrow",
        {x = 416, y = 27, w = 22, h = 27},
        {w = 22, h = 27}
    )
    ui:addBackgroundImage("gradient", "gradient.png")
    ui:setBackgroundImage("gradient")
    for i, v in pairs(players) do
        ui.players[i] = {}
        for k2, v2 in pairs(v) do
            -- if k2 == "name" then
            --     local str = string.format("name: %s", v2)
            --     ui:addText2(ui.players[i], "name_id", str, fg_color, bg_color)
            -- end
            -- if k2 == "age" then
            --     local str = string.format("age: %s", v2)
            --     ui:addText2(ui.players[i], "age_id", str, fg_color, bg_color)
            -- end
            if k2 == "person_id" then
                person = nyq.database.getFromID("people", v2);
                local str = string.format("name: %s", person.name)
                ui:addText2(ui.players[i], "name_id", str, 5, fg_color, bg_color)
                local str = string.format("age: %d", person.age)
                ui:addText2(ui.players[i], "age_id", str, 5, fg_color, bg_color)
            end

        end
    end
    ui:addText("menu", "new_game", "new game", 5, fg_color, bg_color)
end

function ui:update()
    nyq.hasFrameCap(true)
    ui:updateBackground()

    if #players > 0 then
        if ui.selected == 1 then
            ui:updateCrop("load_selected", {x = 16, y = 8})
            ui:updateCrop("box2_unselected", {x = 16, y = 96})
        elseif ui.selected == 2 then
         ui:updateCrop("box1_unselected", {x = 16, y = 8})
         ui:updateCrop("box2_selected", {x = 16, y = 96})
        end
        if (ui.selected2 < #players) then
            if (nyq.getTicks() % 800 < 400) then
                ui:updateCrop("right_arrow", {x = 215, y = 30}, {w = 22 * ui.scale, h = 27 * ui.scale})
            end
        end
        if (ui.selected2 > 1) then
            if (nyq.getTicks() % 800 < 400) then
                ui:updateCrop("left_arrow", {x = 3, y = 30}, {w = 22 * ui.scale, h = 27 * ui.scale})
            end
        end
        ui:updateText2(ui.players[ui.selected2], "name_id", {x = 22, y = 10})
        ui:updateText2(ui.players[ui.selected2], "age_id", {x = 22, y = 20})
        ui:updateText("menu", "new_game", {x = 22, y = 105})
    else
         ui:updateCrop("box2_selected", {x = 16, y = 8})
         ui:updateText("menu", "new_game", {x = 22, y = 18})
    end


end

return ui
