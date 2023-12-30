local ui = {}

function ui:init(spritesheet, window_size, options)
    setmetatable(ui, _ui)
    ui:_init(spritesheet, window_size, "NotoSansMono-Regular.ttf", 12, options, function()
        ui.background.position = {x = 0, y = 0}
        ui.background.size = window_size
        local pos = {x = 0, y = 0}
        ui.images = {}
        ui.images.clonemon = {}
        ui.images.clonemon.position = pos
        ui.company = {}
        ui.company.name = nil
        ui.songPlayed = false
        ui.selection = {}
        ui.selection.continue = nil
        ui.count = 0
        ui.angle = 0
        ui.rotate = 370
        ui.rotate_sign = 1
        ui.start_time = nyq.getTicks()
    end)
end

function ui:companyName()
    nyq.clear({r = 0, g = 0, b = 0, a = 255})
    ui:updateText("company", "name", {x = 60, y = 140})
end

function ui:startCallback(params)
    nyq.hasFrameCap(true)
    -- ui.images.clonemon.id = nyq.images.create("clonemon.png", 0)
    ui.images.clonemon.id = ui:imageCreate("clonemon.png", 0)
    local fg_color = {r = 255, g = 255, b = 204, a = 255}
    local fg_color2 = {r = 0, g = 0, b = 0, a = 255}
    local bg_color = {r = 0, g = 0, b = 0, a = 0}
    local str = "made by NyqSoft"
    ui:addText("company", "name", str, 5, {r = 255, g = 255, b =255, a = 255}, bg_color)
    ui:addText("selection", "continue", "press 'select' to continue", 5, fg_color, bg_color)
    ui:addText("selection", "continue2", "press 'select' to continue", 5, fg_color2, bg_color)
    if ui.songPlayed then
         nyq.audio.stopAll()
         nyq.audio.play("intro")
    end
end

function ui:songPlayIntro()
    if not ui.songPlayed then
         nyq.audio.stopAll()
         nyq.audio.play("intro")
    end
    ui.songPlayed = true
end

function ui:update()
    if nyq.getTicks() - ui.start_time >= 1500 then
        ui:songPlayIntro()
        nyq.clear({r = 255, g = 128, b = 128, a = 255})
        ui.angle = 90
        ui.images.clonemon.position.x = (math.cos(ui.angle ) * 2) + ( 20)
        ui.images.clonemon.position.y = (math.sin(ui.angle ) * 2) + ( 20)

        -- nyq.images.update(ui.images.clonemon.id, ui.images.clonemon.position, {
            --      w = (ui.window_size.w - 50) * ui.scale, h = (ui.window_size.h - 50) * ui.scale
            -- })

        ui:imageUpdate(ui.images.clonemon.id, ui.images.clonemon.position, {
             w = (ui.window_size.w - 50) , h = (ui.window_size.h - 50) 
        })

        nyq.images.rotate(ui.images.clonemon.id, ui.rotate)
        ui.rotate = ui.rotate - (nyq.deltaTime * 30) * ui.rotate_sign

        if ((ui.rotate <= 350) or (ui.rotate >= 370)) then 
            ui.rotate_sign = ui.rotate_sign * -1
        end

        ui:updateText("selection", "continue2", {x = 25 - 1, y = 140 + 1})
        ui:updateText("selection", "continue", {x = 25, y = 140})
        return false
    else
        nyq.clear({r = 0, g = 0, b = 0, a = 255})
        ui:updateText("company", "name", {x = 60, y = 140})
        return true
    end
end

return ui
