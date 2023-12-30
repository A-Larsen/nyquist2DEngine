local ui = {}

function ui:init(window_size, options)
    setmetatable(ui, _ui)
    ui:_init(nil, window_size, "NotoSansMono-Regular.ttf", 8, options, function()
        ui.sprite_ids = {}
        ui.unselected_rect_ids = {}
        ui.selected = 1
    end)
end

function ui:startCallback()
    nyq.hasFrameCap(false)
    ui:addBackgroundImage("background", "gradient.png")
    ui:setBackgroundImage("background")

    ui.max = params.count

    for i = 1, ui.max do
        local w = (ui.window_size.w / ui.max)
        local x = (i - 1) * w
        ui.sprite_ids[i] = ui:newSprite(params.sprite)
        ui.unselected_rect_ids[i]  = ui:createRect({r = 0, g = 0, b = 0, a = 200}, true, 2)
        ui:spriteAddCrops(ui.sprite_ids[i], "choices", 0, { {
            x = x,
            y = 0,
            w = w,
            h = ui.window_size.h
        } })
    end
end

function ui:update()
    ui:updateBackground()

    for i = 1, ui.max do
        local w = (ui.window_size.w / ui.max)
        local x = (i - 1) * w
        if i ~= ui.selected then
            ui:rectUpdate(ui.unselected_rect_ids[i], {
                x = x,
                y = 0,
            }, {
                w = w,
                h = ui.window_size.h
            })
        end
        ui:spriteUpdateCrop(ui.sprite_ids[i], "choices", 1, {x = x, y = 0})
    end
end

return ui
