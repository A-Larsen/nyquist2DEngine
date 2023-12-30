screen_size = {w = 240, h = 160}

nyq.init("sprites", {x = 400, y = 100}, screen_size, {
    scale = 3,
    frame_rate = 60,
    keys = {
        move_north = "W",
        move_east = "D",
        move_south = "S",
        move_west = "A",
        rotate_right = "L",
        rotate_left = "J"
    },
    terminal = {
        font_size = 20, 
        height = 300,
        bg_color = {r = 0, g = 204, b = 0, a = 150},
        error_color = {r = 255, g = 0, b = 255, a = 255},
        cmd_color = {r = 0, g = 255, b = 0, a = 255},
        max_history = 20
    }
})


sprite1_id = nyq.sprites.new("c++.png", {x = 0, y = 0})
nyq.sprites.setHitBox(sprite1_id, {x = 4, y =22}, {w = 40, h = 40})

sprite2_id = nyq.sprites.new("character_red.png", {x = 0, y = 0})
nyq.sprites.setHitBox(sprite2_id, {x = 0, y =1}, {w = 60, h = 40})

nyq.sprites.addCrops(sprite1_id, "test", {
    {x = 0, y = 0, w = 40, h = 40},
    {x = 50, y = 0, w = 40, h = 40},
    {x = 100, y = 0, w = 40, h = 40},
    {x = 150, y = 0, w = 40, h = 40},
    {x = 200, y = 0, w = 40, h = 40},
    {x = 250, y = 0, w = 40, h = 40},
})
--
-- you cannot annimate a crops that have only one frame
nyq.sprites.addCrops(sprite2_id, "idle_north", {
    {x = 17, y = 0, w = 15, h = 19},
})

nyq.sprites.addCrops(sprite2_id, "idle_east", {
    {x = 17, y = 19, w = 15, h = 19},
})

nyq.sprites.addCrops(sprite2_id, "idle_south", {
    {x = 17, y = 38, w = 15, h = 19},
})

nyq.sprites.addCrops(sprite2_id, "idle_west", {
    {x = 17, y = 57, w = 15, h = 19},
})

nyq.sprites.addCrops(sprite2_id, "walk_south", {
    {x = 32, y = 38, w = 15, h = 19},
    {x = 49, y = 38, w = 15, h = 19},
})

nyq.sprites.addCrops(sprite2_id, "walk_north", {
    {x = 32, y = 0, w = 15, h = 19},
    {x = 49, y = 0, w = 15, h = 19},
})

nyq.sprites.addCrops(sprite2_id, "walk_east", {
    {x = 32, y = 19, w = 15, h = 19},
    {x = 49, y = 19, w = 15, h = 19},
})

nyq.sprites.addCrops(sprite2_id, "walk_west", {
    {x = 32, y = 58, w = 15, h = 19},
    {x = 49, y = 58, w = 15, h = 19},
})

pos = {x = 0, y = 0}

idle = "idle_north"

degrees = 0
bg_color = {r = 0, g = 0, b = 0, a = 0}

-- tempPos = pos

-- set = false
terminal = {
    temps = {
        pos =  pos
    },
    bools = {
        pos = false
    },
    commands = {
        setPosition = function(x, y)
            synopsis = "synopsis: setPosition(<number>, <number>)"
            if (type(x) ~= "number" or type(y) ~= "number")  then
                nyq.terminal.error(synopsis)
            else 
                stdout = string.format("position set at x = %d, y = %d", x, y)
                stderr = string.format("could not set position x = %d, y = %d", x, y)
                if (x > screen_size.w or y > screen_size.h) then
                    nyq.terminal.error(stderr)
                else
                    -- eventually need to handle new lines..
                    terminal.temps.pos = {x = x, y = y}
                    nyq.terminal.message(
                    [[this message is designed just to see what happends when you type a really long message, it needs to be several lines long in order to see if it can wrap the text correctly. At this point I am not really sure how many lines this is.. quite a bit I would suppose]])
                end
               terminal.bools.pos = true

            end

        end
    }
}

function logicLoop()
    animation = nil

    if terminal.bools.pos then
        pos = terminal.temps.pos
        terminal.bools.set = false
    end

    if (nyq.keypress.move_north) then
        idle = "idle_north"
        animation = "walk_north"
        pos.y = pos.y - 1
    elseif (nyq.keypress.move_east) then
        idle = "idle_east"
        animation = "walk_east"
        pos.x = pos.x + 1
    elseif (nyq.keypress.move_west) then
        idle = "idle_west"
        animation = "walk_west"
        pos.x = pos.x - 1
    elseif (nyq.keypress.move_south) then
        idle = "idle_south"
        animation = "walk_south"
        pos.y = pos.y + 1
    end

    if (nyq.keypress.rotate_right) then
        if degrees <= 360 then
            degrees = degrees + 1
        end
        if degrees >= 360 then
            degrees = 0
        end
    elseif (nyq.keypress.rotate_left) then
        if degrees == 0 then
            degrees = 360 - 1
        else
            degrees = degrees - 1
        end
    end
end

function drawLoop()
    -- nyq.loops.loopStart(loop_id)

    nyq.clear(bg_color)

    if animation then
        frame_number = nyq.sprites.animate(sprite2_id, animation, 20, pos)
        frame_number2 = nyq.sprites.getCurrentFrame(sprite2_id, animation);
        -- print("animation frame number: ", frame_number)
    else
        nyq.sprites.updateCrop(sprite2_id, idle, 1, pos)
        frame_number2 = nyq.sprites.getCurrentFrame(sprite2_id, idle);
    end
    nyq.sprites.rotateCrop(sprite2_id, degrees)
    -- nyq.loops.loopEnd(loop_id)

end
