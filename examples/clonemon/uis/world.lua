local ui = {}

function ui:init(player_spritesheet, world_name, world_spritesheet, window_size, options)
    setmetatable(ui, _ui)
    ui:_init(player_spritesheet, window_size, "NotoSansMono-Regular.ttf", 8, options, function()
        -- nyq.world.init(world_spritesheet, world_name);
        ui.world_spritesheet = world_spritesheet
        ui.world_name = world_name
        ui.player = {}
        ui.player.scale = 3
        ui.player.position = {}
        ui.player.position.x = 0
        ui.player.position.y = 0
        ui.move = nil
        ui.player.speed = 600
        ui.player.idle = "idle_north"
        ui.player.move = nil
        ui.player.screen_position = {}
        ui.selection_max = 0

        -- ui.player.screen_position = ui:setPosition({
        --     x = ui.window_size.w / 2,
        --     y = ui.window_size.h / 2 + 4

        -- })
        ui.player.screen_position = {
            x = ui.window_size.w / 2,
            y = ui.window_size.h / 2 + 4
            -- x = 0,
            -- y = 0

        }

        ui.map = {}
        ui.map.position = {}

        print("world ui.scale: " .. ui.scale)

        ui.map.zIndex = 0
        ui.player.hitbox  = {}
        -- ui.player.screen_position.y = ui.scaled_window_size.h / 2 + 4

        ui.player.hitbox = ui:setSize({
            w = 13 ,
            h = 14

        })

        ui.playerObjectCollision = nil
        ui.start_menu = {}
        local pos = { x = 184, y = 4 }
        ui.start_menu.position = pos


        ui.start_menu.selection = {}
        ui.start_menu.size = {w = 0, h = 0}
    end)
end

function ui:startCallback()
    nyq.hasFrameCap(true)
    ui.selected = 1
    ui.selection_rect = ui:createRect({r = 100, g = 0, b = 0, a = 50}, true, 6)
    ui.text_rect_id  = ui:createRect({r = 0, g = 0, b = 0, a = 200}, true, 4)
    -- ui.option_rect_id  = ui:createRect({r = 225, g = 255, b = 255, a = 200}, true, 4)
    -- ui.menu_items_id = ui:newSprite("menu_items.png")
    -- ui:spriteAddCrops(ui.menu_items_id, "selection arrow", 6, {
    --     {
    --         x = 7,
    --         y = 0,
    --         w = 7,
    --         h = 7
    --     }
    -- })
    ui.text_pos = 0
    ui.text_max = 0
    ui.text_done = false
    ui.text_end = false

    -- for i = 1, 6 do
    --     ui:addText("options", i , "yes", 5,
    --         {r = 0, g = 0, b = 0, a = 255},
    --         {r = 0, g = 0, b = 0, a = 0}
    --     )
    -- end

    for i = 1, 3 do
        local id = string.format("%d", i)
        ui:addText("text", id, " ", 5,
            {r = 255, g = 255, b = 255, a = 255},
            {r = 0, g = 0, b = 0, a = 0}
        )
    end

    nyq.world.init(ui.world_spritesheet, ui.world_name);
    nyq.world.read(params.world_id, ui.map.zIndex)
    ui:setPlayerScale(70)
    ui.face_sprite_id = ui:newSprite(ui.spritesheet)
    ui:spriteAddCrops(ui.face_sprite_id, "face", 5, {
        {
            x = 0,
            y = 173,
            w = 50,
            h = 30
        }
    })
    ui.player.colliding_id = nyq.world.addCollisionCheck({
        x = ui.player.screen_position.x * ui.scale + (ui.scale * 1),
        y = ui.player.screen_position.y * ui.scale + (ui.scale * 7)
    }, ui.player.hitbox)


    local size = {w = 44 * .35, h = 57 * .35}

    ui:addAnimation("player", "walk_north", {
        {x = 0, y = 0, w = 44, h = 57},
        {x = 0, y = 58, w = 44, h = 57},
        {x = 0, y = 0, w = 44, h = 57},
        {x = 0, y = 116, w = 44, h = 57},
    }, size)

    ui:addAnimation("player", "idle_north", {
        {x = 0, y = 0, w = 44, h = 57},
    }, size)

    ui:addAnimation("player", "walk_east", {
        {x = 44, y = 0, w = 44, h = 57},
        {x = 44, y = 57, w = 44, h = 57},
        {x = 44, y = 116, w = 44, h = 57},
    }, size)

    ui:addAnimation("player", "idle_east", {
        {x = 44, y = 0, w = 44, h = 57},
    }, size)

    ui:addAnimation("player", "walk_south", {
        {x = 88, y = 0, w = 44, h = 57},
        {x = 88, y = 58, w = 44, h = 57},
        {x = 88, y = 0, w = 44, h = 57},
        {x = 88, y = 116, w = 44, h = 57},
    }, size)

    ui:addAnimation("player", "idle_south", {
        {x = 88, y = 0, w = 44, h = 57},
    }, size)

    ui:addAnimation("player", "walk_west", {
        {x = 132, y = 0, w = 44, h = 57},
        {x = 132, y = 58, w = 44, h = 57},
        {x = 132, y = 116, w = 44, h = 57},
    }, size)

    ui:addAnimation("player", "idle_west", {
        {x = 132, y = 0, w = 44, h = 57},
    }, size)
    ui.menu_size = #params.menu_items
    ui.start_menu.id = ui:imageCreate("pause.png", 5)

    local padding = 11
    local padding_top_and_bottom = 8
    local fg_color = {r = 0, g = 0, b = 0, a = 0}
    local bg_color = {r = 255, g = 255, b = 255, a = 0}

    for i, item in ipairs(params.menu_items) do
        ui.start_menu.selection[i] = {}
        ui.start_menu.selection[i].position = {x = 187 + 5, y = padding_top_and_bottom  + padding * (i - 1)}
        ui.start_menu.selection[i].id = nil
        ui:addText2(ui.start_menu.selection[i], "id", item.name, 5, fg_color, bg_color)
    end
    local text_size = ui:getTextSize2(ui.start_menu.selection[1], "id")
    ui.start_menu.size = {
                w = 53, h = ((padding * (ui.menu_size - 1)) +
                (padding_top_and_bottom)) + text_size.h
    }
    nyq.audio.stopAll()
    nyq.audio.play("world")
end

function ui:setPlayerScale(scale)
    -- scale HAS to be a whole number
    ui.player.scale = (1 / nyq.world.getGrid()) * scale
end


function ui:setPlayerSpeed(speed)
    ui.player.speed = speed
end

function ui:setPlayerPosition(position)
    -- REMEMBER: look at the width and height of the map config, NOT the
    -- png width and height
    ui.map.position.x = -((position.x - ui.player.screen_position.x + 14) * 5 * 2)
    ui.map.position.y = -((position.y - ui.player.screen_position.y + 11) * 5 * 2)
end

function ui:moveNorth()
    ui.player.idle = "idle_north"
    ui.player.move = "walk_north"
    local distance = (ui.player.speed * nyq.deltaTime)
    if (ui.playerObjectCollision and ui.playerObjectCollision.is_collidable) then
        if (ui.playerObjectCollision.hit_bottom) then
            return
        end
    end
    ui.player.position.y = ui.player.position.y + distance
    ui.map.position.y = ui.map.position.y + distance
end

function ui:moveEast()
    ui.player.idle = "idle_east"
    ui.player.move = "walk_east"
    local distance = (ui.player.speed * nyq.deltaTime)

    if (ui.playerObjectCollision and ui.playerObjectCollision.is_collidable) then
        if (ui.playerObjectCollision.hit_left) then
            return
        end
    end

    ui.player.position.x = ui.player.position.x + distance
    ui.map.position.x = ui.map.position.x - distance
end

function ui:moveSouth()
    ui.player.idle = "idle_south"
    ui.player.move = "walk_south"
    local distance = (ui.player.speed * nyq.deltaTime)
    if (ui.playerObjectCollision and ui.playerObjectCollision.is_collidable) then
        if (ui.playerObjectCollision.hit_top) then
            return
        end
    end
    ui.player.position.y = ui.player.position.y - distance
    ui.map.position.y = ui.map.position.y - distance
end

function ui:moveWest()
    ui.player.idle = "idle_west"
    ui.player.move = "walk_west"
    local distance = (ui.player.speed * nyq.deltaTime)
    if (ui.playerObjectCollision and ui.playerObjectCollision.is_collidable) then
        if (ui.playerObjectCollision.hit_right) then
            return
        end
    end
    ui.player.position.x = ui.player.position.x - distance
    ui.map.position.x = ui.map.position.x + distance
end


function ui:collisionCheck()
    ui.playerObjectCollision = nyq.world.checkCollision(ui.player.colliding_id)
end

function ui:worldUpdate()
    -- do NOT load anything fram a group in here!
    -- it will confilict with other uis
    if ui.player.move then
        local speed = 20
        if ui.player.move == "walk_north" or ui.player.move == "walk_south" then
            speed = 27
        end
        ui:updateAnimation("player", ui.player.move, speed,  ui.player.screen_position)
    else
        ui:updateAnimation("player", ui.player.idle, 1, ui.player.screen_position)
    end
    --
    nyq.players.set(ui.player.position, 0)
    nyq.map.set(ui.map.position)
    nyq.world.update(1 + ((ui.scale - ui.player.scale) / ui.player.scale))
    -- nyq.world.update(1)
    -- nyq.world.update(1 / ui.scale)
    ui.player.move = nil
end

function ui:pauseUpdate()

    ui:imageUpdate(ui.start_menu.id, ui.start_menu.position, ui.start_menu.size)

    local size = ui:getTextSize2(ui.start_menu.selection[ui.selected], "id")
    local text_pos = ui.start_menu.selection[ui.selected].position
    ui:rectUpdate(ui.selection_rect,
                    {x = (ui.start_menu.position.x) + (4),
                     y = (text_pos.y)},
                     {w = (ui.start_menu.size.w) - (9),
                     h = size.h})

    for i, selection in ipairs(ui.start_menu.selection) do
        ui:updateText2(selection, "id", selection.position)
    end

end

-- function OptionSelectUpdate(options, start_y, rect_id, selection_id, selected)
--     ui.selection_max = #options
--     local height = 1
--     local font_offset = ui.font_size + 1
--     local max_width = {
--         len = 0,
--         id = 0
--     }

--     for i, option in pairs(options) do
--         local len = string.len(option)
--         if len > max_width.len then
--             max_width.len = len
--             max_width.id = i
--         end
--     end

--     local size = ui:getTextSize("options", max_width.id)
--     local rect_pos = {x = 225 - size.w, y = 128}

--     local rev = table.Reverse(options)

--     for i, option in pairs(rev) do
--         local y_offset = (i - 1) * font_offset
--         height = height + font_offset
--         rect_pos.y = rect_pos.y - font_offset
--         ui:textChange("options", option.id, option.text,
--             {r = 0, g = 0, b = 0, a = 255},
--             {r = 0, g = 0, b = 0, a = 0}
--         )
--         ui:updateText("options", option.id,  {
--             x = 238 - size.w,
--             -- y = 118 - y_offset
--             y = start_y - y_offset
--         })
--     end
--     -- ui:rectUpdate(ui.option_rect_id,
--     ui:rectUpdate(rect_id,
--     rect_pos,
--     {
--         w = size.w + 14,
--         h = height
--     })
--     -- ui:spriteUpdateCrop(ui.menu_items_id, "selection arrow", 1, {
--     ui:spriteUpdateCrop(selection_id, "selection arrow", 1, {
--         x = 238 - size.w - 10,
--         -- y = (121 + font_offset - height) + (font_offset * (ui.selected - 1))
--         y = (121 + font_offset - height) + (font_offset * (selected - 1))
--     })
-- end

function ui:optionSelectUpdate(options)
    -- self:optionSelect(options, 118, ui.selected)
    self:optionSelect(options, {x = 238, y = 118}, ui.selected)
end

-- function ui:textYesNoUpdate()
--     ui:optionSelectUpdate({"yes", "no", "maybe", "suck it", "no one really cares about that"})
--     -- ui:optionSelectUpdate({"yes", "no"})
-- end

function ui:textUpdate(text)
    if ui.text_end then
        ui.text_end = false
        return
    end

    local lines = string.WordWrap(text, 38)
    ui.text_max = math.ceil(#lines / 3)

    for i = 1, 3 do
        if not lines[i + (ui.text_pos * 3)] then
            ui.text_done = true
            break
        end
        local id = string.format("%d", i)
        local y = 130 + ((i - 1) * (ui.font_size + 1))
        ui:textChange("text", id, lines[i + (ui.text_pos * 3)],
            {r = 255, g = 255, b = 255, a = 255},
            {r = 0, g = 0, b = 0, a = 0}
        )
        ui:updateText("text", id, {x = 54,  y = y})
    end

    ui:spriteUpdateCrop(ui.face_sprite_id, "face", 1, {
        x = 0, y = 130
    })
    ui:rectUpdate(ui.text_rect_id,
    {
        x = 50,
        y = 130
    },
    {
        w = 190,
        h = 30
    })
end


return ui
