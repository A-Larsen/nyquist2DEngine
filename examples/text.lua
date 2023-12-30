nyq.init("text", {x = 400, y = 100}, {w = 800, h = 400})
-- nyq.text.init("NotoSansMono-Regular.ttf", 50)

text_pos = {x = 240 / 3, y = 160 / 3}


keypress = nyq.events.init({
    move_north = "W",
    move_east = "D",
    move_south = "S",
    move_west = "A",
})

text_list = {"hey!", "no", "what is up!!", "?"}

color_list = {
        {r = 255, g = 255, b = 0, a = 0},
        {r = 0, g = 255, b = 0, a = 0},
        {r = 0, g = 255, b = 255, a = 0},
        {r = 255, g = 255, b = 255, a = 0},
}

i = 1
text = "what"
text_id = nyq.text.create(
    "NotoSansMono-Regular.ttf", 50,
    text, 
    color_list[i],
    {r = 0, g = 0, b = 0, a = 0}
)
size = nyq.text.getSize(text_id)

function eventLoop()
    nyq.events.update(keypress)
    if nyq.events.trigger(keypress.move_north) then
        print("north")
        i = 1
        nyq.text.change(
            text_id,
            text_list[i], 
            color_list[i],
            {r = 0, g = 0, b = 0, a = 0}
        )
    elseif nyq.events.trigger(keypress.move_south) then
        print("south")
        i = 2
        nyq.text.change(
            text_id,
            text_list[i], 
            color_list[i],
            {r = 0, g = 0, b = 0, a = 0}
        )
    elseif nyq.events.trigger(keypress.move_west) then
        print("west")
        i = 3
        nyq.text.change(
            text_id,
            text_list[i], 
            color_list[i],
            {r = 0, g = 0, b = 0, a = 0}
        )
    elseif nyq.events.trigger(keypress.move_east) then
        print("east")
        i = 4
        nyq.text.change(
            text_id,
            text_list[i], 
            color_list[i],
            {r = 0, g = 0, b = 0, a = 0}
        )
    end
end

function gameLoop()
    nyq.clear({r = 0, g = 0, b = 0, a = 0})


    size = nyq.text.getSize(text_id)
    color = {r = 255, g = 0, b = 0, a = 0}
    nyq.drawRect({x = text_pos.x - 1, y = text_pos.y - 1},
                 {w = size.w + 2, h = size.h + 2}, color, false)

    nyq.text.update(text_id, text_pos)

    -- the delay is necessary for saving cpu usage!
    nyq.delay(20)
end

