local o = {}

o.scale = 1
o.window_size = {w = 0, h = 0}
o.background = {
    position = {x = 0, y = 0},
    size = {w = 0, h = 0},
    images = {},
}
o.spritesheet_id = 0
o.spritesheet = nil
o.ttf_file = nil
o.font_size = nil
o.sprite_ids = {}
o.image_ids = {}

function o:setSize(size)
    local s = {
        w = size.w * self.scale,
        h = size.h * self.scale
    }
    return s
end

function o:setPosition(pos)
    local p = {
        x = pos.x * self.scale,
        y = pos.y * self.scale,
    }
    return p
end

function o:_scaleRect(rect)
local r = {
        x = rect.x * self.scale,
        y = rect.y * self.scale,
        w = rect.w * self.scale,
        h = rect.h * self.scale,
    }
    return r
end

function o:addCrop(name, crop, size)
    if not self[name] then
        self[name] = {}
    end
    self[name].crop = crop
    self[name].size = size
    nyq.sprites.addCrops(self.spritesheet_id, name, 0, {self[name].crop})
end

function o:addAnimation(name, id, crops, size)
    if (not self[name]["crops"]) then
        self[name]["crops"] = {}
    end
    if (not self[name]["crops"][id]) and crops then
        self[name]["crops"][id] = crops
    end
    nyq.sprites.addCrops(self.spritesheet_id, id, 1, self[name]["crops"][id])
    self[name].size = size
end

function o:updateAnimation(name, id, speed, pos)
    local s = self:setSize(self[name].size)
    -- local p = {
    --     x = pos.x ,
    --     y = pos.y
    -- }
    local p = self:setPosition(pos)
    self[name].frame_number = 
        nyq.sprites.animate(self.spritesheet_id, id, speed, p, s)
end

function o:updateCrop(name, pos)
    local s = self:setSize(self[name].size)
    local p = self:setPosition(pos)
    self[name].position = pos
    nyq.sprites.updateCrop(self.spritesheet_id,name, 1, p, {size = s})
end

function o:updateCrop2(name, id, pos)
    local s = self:setSize(self[name].size)
    local p = {
        x = pos.x,
        y = pos.y
    }
    self[name].frame_number = 
        nyq.sprites.updateCrop(self.spritesheet_id, id, 1, p, {size = s})
end

function o:addBackgroundImage(name, path)
    self.background.images[name] = nyq.images.create(path, 0)
    table.insert(self.image_ids, self.background.images[name])
end

function o:setBackgroundImage(image)
    if self.background.images[image] then
        self.background_image = self.background.images[image]
        return true
    end

    return false
end

function o:updateBackground()
    local s = self:setSize(self.background.size)
    local p = self:setPosition(self.background.position)
    nyq.images.update(self.background_image, p, s)
end

function o:setFont(ttf_file, size)
    self.ttf_file = ttf_file
    self.font_size = size
end

function o:addText(name, id, string, zIndex, fg_color, bg_color)
    if not self[name] then
        self[name] = {}
    end

    self[name].string = string

    if not self[name][id]  then
        self[name][id] = nyq.text.create(
            self.ttf_file,
            self.font_size * self.scale,
            self[name].string,
            zIndex,
            fg_color,
            bg_color
        )
    else
        nyq.text.change(
            self[name][id],
            self[name].string,
            fg_color,
            bg_color
        )
    end
    table.insert(self._free_me, {name, id})
end

function o:clearText(name, id)
    if not name then return end
    self[name][id] = nil
end

function o:clearText2(o, id)
    if not o then return end
    o[id] = nil
end

function o:addText2(o, id, string, zIndex, fg_color, bg_color)
    if not o[id] then
        o[id] = nyq.text.create(
            self.ttf_file,
            self.font_size * self.scale,
            string,
            zIndex,
            fg_color,
            bg_color
        )
    else
        nyq.text.change(
            o[id],
            string,
            fg_color,
            bg_color
        )
    end
    table.insert(self._free_me, {o, id})
end

function o:free()
    for _, v in pairs(self._free_me) do
        if type(v[1]) == "string" then
            self[ v[1] ][ v[2] ] = nil
        elseif type(v[1]) == "table" then
            -- k[v] = nil
            v[1][ v[2] ] = nil
        end
    end

    nyq.text.clear()

    if self.freeCallback then
        self.freeCallback()
    end

    nyq.sprites.free()
    nyq.images.free()
    nyq.world.free()
    nyq.rects.free()

end

function o:textChange(name, id, text, forground_color, background_color)
    nyq.text.change(
        self[name][id],
        text,
        forground_color,
        background_color
    )
end

function o:textChange2(o, id, text, forground_color, background_color)
    nyq.text.change(
        o[id],
        text,
        forground_color,
        background_color
    )
end

function o:updateText2(o, id, position)
    if (not o) then return end
    if (not o[id]) then return end
    local p = self:setPosition(position)
    nyq.text.update(o[id], p)
end

function o:updateText(name, id, position)
    if (not self[name]) then return end
    if (not self[name][id]) then return end

    -- if not self[name].text then
    --     self[name].text = {}
    -- end

    -- self[name].text.position = position
    local p = self:setPosition(position)
    if (self[name].string) then
        nyq.text.update(self[name][id], p)
    end
end

function o:getTextSize(name, id)
    if (not self[name]) then return end
    if (not self[name][id]) then return end
    -- if not self[name].text then
    --     self[name].text = {}
    -- end
    local size = nyq.text.getSize(self[name][id])
    return {
        w = size.w / self.scale,
        h = size.h / self.scale,
    }
end

function o:getTextSize2(o, id)
    if (not o) then return end
    if (not o[id]) then return end
    local size = nyq.text.getSize(o[id])
    return {
        w = size.w / self.scale,
        h = size.h / self.scale,
    }
end

function o:newSprite(file)
    local id = nyq.sprites.new(file)
    table.insert(self.sprite_ids, id)
    return id
end

function o:spriteAddCrops(id, id2, zIndex, frames)
    nyq.sprites.addCrops(id, id2, zIndex, frames)
end

function o:spriteUpdateCrop(id, id2, idx, pos, scale)
    local p = self:setPosition(pos)
    if scale then
        local s = self:setSize(scale)
        nyq.sprites.updateCrop(id, id2, idx, p, {size = s})
    else
        nyq.sprites.updateCrop(id, id2, idx, p, {
            scale = {
                w = self.scale,
                h = self.scale
            }
        })
    end
end

function o:imageCreate(file, zIndex)
    local id = 0
    id = nyq.images.create(file, zIndex)
    table.insert(self.image_ids, id)
    return id
end

function o:imageUpdate(id, pos, size)
    local p = self:setPosition(pos)
    local s = self:setSize(size)
    nyq.images.update(id, p, s)
end

function o:_init(spritesheet, window_size, ttf_file, font_size, options, callback)

    if spritesheet then
        self.spritesheet = spritesheet
    end

    if options then
        if options.scale then
            self.scale = options.scale
        end
        if options.pos then
            self.pos = options.pos
        end
        if options.background_position then
            self.background.position = options.background_position
        end
    end
    self:setFont(ttf_file, font_size)
    self.window_size = window_size
    self.scaled_window_size = {w = window_size.w * self.scale, h = window_size.h * self.scale}
    self.background.size = window_size 
    self._free_me = {}
    local p = self:setPosition(self.background.position)
    self.formats_groud_id = nyq.formats.newGroup()
    callback()
    nyq.formats.saveGroup(self.formats_groud_id)
end

function o:createRect(color, fill, zIndex)
    return nyq.rects.create(color, fill, zIndex)
end

function o:rectUpdate(id, pos, size)
    local p = self:setPosition(pos)
    local s = self:setSize(size)
    nyq.rects.update(id, p, s)
end

function o:start(params)
    nyq.formats.loadGroup(self.formats_groud_id)

    if self.spritesheet then
        self.spritesheet_id = nyq.sprites.new(self.spritesheet)
    end

    for i = 1, 6 do
        self:addText("options", i , "yes", 5,
            {r = 0, g = 0, b = 0, a = 255},
            {r = 0, g = 0, b = 0, a = 0}
        )
    end
    self.option_rect_id  = self:createRect({r = 225, g = 255, b = 255, a = 200}, true, 4)
    self.menu_items_id = self:newSprite("menu_items.png")
    self:spriteAddCrops(self.menu_items_id, "selection arrow", 6, {
        {
            x = 7,
            y = 0,
            w = 7,
            h = 7
        }
    })
    self.params = params
    if self.startCallback then
        self.startCallback()
    end
end

function o:linesCreate(name, count, fg_color, bg_color)
    self[name].line_count = count
    self[name].line_ids = {}
    self[name].page_idx = 0
    self[name].pages = 0
    self[name].fg_color = fg_color
    self[name].bg_color = bg_color

    for i = 1, count do
        self:addText2(
            self[name].line_ids,
            i,
            "lines", -- cannot leave blank
            5,
            self[name].fg_color,
            self[name].bg_color
        )
    end
end

function o:optionSelect(options, start_pos, selected)
    self.selection_max = #options
    local height = 1
    local font_offset = self.font_size + 1
    local max_width = {
        len = 0,
        id = 0
    }

    for i, option in pairs(options) do
        local len = string.len(option)
        if len > max_width.len then
            max_width.len = len
            max_width.id = i
        end
    end

    local size = self:getTextSize("options", max_width.id)
    -- local rect_pos = {x = 225 - size.w, y = 128}
    local rect_pos = {x = start_pos.x - size.w - 12, y = start_pos.y + 8}

    local rev = table.Reverse(options)

    for i, option in pairs(rev) do
        local y_offset = (i - 1) * font_offset
        height = height + font_offset
        rect_pos.y = rect_pos.y - font_offset
        self:textChange("options", option.id, option.text,
            {r = 0, g = 0, b = 0, a = 255},
            {r = 0, g = 0, b = 0, a = 0}
        )
        self:updateText("options", option.id,  {
            -- x = 238 - size.w,
            x = start_pos.x- size.w,
            y = start_pos.y - y_offset
        })
    end
    self:rectUpdate(self.option_rect_id,
    rect_pos,
    {
        w = size.w + 14,
        h = height + 2
    })
    self:spriteUpdateCrop(self.menu_items_id, "selection arrow", 1, {
        x = start_pos.x - size.w - 10,
        y = (start_pos.y + 3 + font_offset - height) + (font_offset * (selected - 1))
    })
end

function o:linesDraw(name, text, len, position)
    local lines = string.WordWrap(text, len)
    self[name].pages = math.ceil(#lines / self[name].line_count)
    local max = #lines
    if max > self[name].line_count then max = self[name].line_count end
    for i = 1, max do
        if not lines[i + (self[name].page_idx * self[name].line_count)] then
            return true
        end
        local y = position.y + ((i - 1) * (self.font_size + 1))
        self:textChange2(self[name].line_ids, i, lines[i + (self[name].page_idx *
                                                 self[name].line_count)],
                        self[name].fg_color,
                        self[name].bg_color)

        self:updateText2(self[name].line_ids, i, {
            x = position.x,
            y = y
        })
    end
    return false
end

return {__index = o}
