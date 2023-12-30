local ui = {}

function ui:init(window_size, options)
    setmetatable(ui, _ui)
    ui:_init(nil, window_size, "NotoSansMono-Regular.ttf", 8, options, function()
        ui.line_count = 7
        ui.description_line_ids = {}
    end)
end

function ui:startCallback()
    nyq.hasFrameCap(false)
    ui.selection_rect = ui:createRect({r = 100, g = 0, b = 0, a = 50}, true, 6)
    ui.menu_items_id = ui:newSprite("menu_items.png")
    ui:spriteAddCrops(ui.menu_items_id, "transparent_arrow_left", 6, {
        {
            x = 14,
            y = 0,
            w = 7,
            h = 7
        }
    })
    ui:spriteAddCrops(ui.menu_items_id, "transparent_arrow_right", 6, {
        {
            x = 21,
            y = 0,
            w = 7,
            h = 7
        }
    })
    ui.selected = 1
    ui.types = {}
    local i = 1
    for k, v in pairs(params) do
        ui.types[i] = k
        i = i + 1
        ui[k] = {
            text_ids = {},
            description_lines = {}
        }
    end
    ui.type_selected = 1
    ui.type_section_max = #ui.types

    ui.type_selected = 1
    for i = 1, ui.line_count do
        ui:addText2(
            ui.description_line_ids,
            i,
            "description", -- blank string here causes crashing
            5,
            {r = 0, g = 0, b = 0, a = 255},
            {r = 0, g = 0, b = 0, a = 0}
        )
    end

    ui:addBackgroundImage("gradient", "gradient.png")
    ui:setBackgroundImage("gradient")
    ui.bag_id = ui:imageCreate("bag.png", 1)

    for _, type in pairs(ui.types) do
        ui:addText("label", type, type, 5,
            {r = 0, g = 0, b = 0, a = 150},
            {r = 0, g = 0, b = 0, a = 0}
        )
        for i, item in pairs(params[type]) do
            ui[type].text_ids[i] = {}
            ui:addText2(ui[type].text_ids[i], "name", item.name, 5,
                {r = 0, g = 0, b = 0, a = 150},
                {r = 0, g = 0, b = 0, a = 0}
            )
            ui[type].description_lines[i] = {}
            ui[type].description_lines[i] = string.WordWrap(item.description, 20)
            ui:addText2(ui[type].text_ids[i], "quantity", string.format("x%d", item.quantity), 5,
                {r = 0, g = 0, b = 0, a = 150},
                {r = 0, g = 0, b = 0, a = 0}
            )
        end
    end
end

function ui:updateBag()
    local type = ui.types[ui.type_selected]
    ui:updateBackground()
    ui:imageUpdate(ui.bag_id, {x = 0, y = 0}, {w = ui.window_size.w, h = ui.window_size.h})
    local itembox_width = 118
    local padding = 2
    local size = ui:getTextSize("label", type)
    if nyq.getTicks() % 600 <= 300 then
        ui:spriteUpdateCrop(ui.menu_items_id, "transparent_arrow_left", 1, {
            x = 7,
            y = 8
        })
        ui:spriteUpdateCrop(ui.menu_items_id, "transparent_arrow_right", 1, {
            x = 107,
            y = 8
        })
    end
    ui:updateText("label", type, {
            x = padding + (itembox_width / 2) - (size.w / 2), y = 6
    })

    local ys = {}
    for i, item in pairs(ui[type].text_ids) do
        local y = 18 + ((i - 1) * (ui.font_size + 1))
        ys[i] = y + 1
        ui:updateText2(ui[type].text_ids[i], "name", {
            x = 130, y = y
        })
        ui:updateText2(ui[type].text_ids[i], "quantity", {
            x = 210, y = y
        })
    end
    local size = ui:getTextSize2(ui[type].text_ids[ui.selected], "name")
    ui.selection_max = #ui[type].text_ids
    ui:rectUpdate(ui.selection_rect,
                    {x = 130,
                     y = ys[ui.selected]},
                     {w = 99,
                     h = ui.font_size + 2})


    for i, line in pairs(ui[type].description_lines[ui.selected]) do
        ui:textChange2(ui.description_line_ids, i, line,
            {r = 0, g = 0, b = 0, a = 150},
            {r = 0, g = 0, b = 0, a = 0}
        )
        local y = 74 + ((i - 1) * (ui.font_size + 1))
        ui:updateText2(ui.description_line_ids, i, {
            x = 8, y = y
        })
    end
end

return ui
