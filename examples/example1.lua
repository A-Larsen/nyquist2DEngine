nyq.init("one", {x = 400, y = 100}, {w = 240, h = 160}, {scale = 3})

circle_pos = {x = 240 / 2, y = 20.0}

text1_pos = {x = 240 / 3, y = 160 / 3}
text2_pos = {x = 10, y = 160 / 3}


image1_pos = {x = 0, y = 0}
image2_pos = {x = 0, y = 0}

image1_id = nyq.images.create("c++.png", image1_pos, { w = 100, h = 100})
image2_id = nyq.images.create("c++.png", image2_pos, { w = 10, h = 10})

keypress = nyq.events.init({
    start = "Escape",
    select = "Return",
    run_or_cancel = "K",
    bike = "L",
    move_north = "W",
    move_east = "D",
    move_south = "S",
    move_west = "A",
    up = "Up",
    down = "Down",
    left = "Left",
    right = "Right"
})

text_1_id = nyq.text.create(
    "NotoSansMono-Regular.ttf", 50,
    "hello!", 
    {r = 255, g = 255, b = 255, a = 0},
    {r = 0, g = 0, b = 0, a = 0}
)

text2_id = nyq.text.create(
    "NotoSansMono-Regular.ttf", 20,
    "goodbye!", 
    {r = 0, g = 255, b = 0, a = 0},
    {r = 0, g = 0, b = 0, a = 0}
)
text3_id = nyq.text.create(
    "NotoSansMono-Regular.ttf", 40,
    "nope!", 
    {r = 0, g = 255, b = 255, a = 0},
    {r = 0, g = 0, b = 0, a = 0}
)
while not nyq.quit do
    nyq.clear({r = 0, g = 0, b = 0, a = 0})

    if keypress.run_or_cancel then
        nyq.images.update(image1_id, image1_pos)
        if keypress.move_north then
            image1_pos.y = image1_pos.y - 1
        elseif keypress.move_south then
            image1_pos.y = image1_pos.y + 1
        elseif keypress.move_west then
            image1_pos.x = image1_pos.x - 1
        elseif keypress.move_east then
            image1_pos.x = image1_pos.x + 1
        end
    end


    if keypress.run_or_cancel then
        nyq.images.update(image2_id, image2_pos) if keypress.up then image2_pos.y = image2_pos.y - 1
        elseif keypress.down then
            image2_pos.y = image2_pos.y + 1
        elseif keypress.left then
            image2_pos.x = image2_pos.x - 1
        elseif keypress.right then
            image2_pos.x = image2_pos.x + 1
        end
    end

    nyq.drawCircle(circle_pos, {r = 255, g = 0, b = 0, a = 0}, 10)

    nyq.events.update(keypress)

    if keypress.move_north then
        circle_pos.y = circle_pos.y - 1
    elseif keypress.move_south then
        circle_pos.y = circle_pos.y + 1
    elseif keypress.move_west then
        circle_pos.x = circle_pos.x - 1
    elseif keypress.move_east then
        circle_pos.x = circle_pos.x + 1
    end

    if keypress.select then
        nyq.text.update(text_1_id, text1_pos)
        if keypress.move_north then
            text1_pos.y = text1_pos.y - 1
        elseif keypress.move_south then
            text1_pos.y = text1_pos.y + 1
        elseif keypress.move_west then
            text1_pos.x = text1_pos.x - 1
        elseif keypress.move_east then
            text1_pos.x = text1_pos.x + 1
        end
    end

    if keypress.bike then
        nyq.text.update(text2_id, text2_pos)
        if keypress.move_north then
            text2_pos.y = text2_pos.y - 1
        elseif keypress.move_south then
            text2_pos.y = text2_pos.y + 1
        elseif keypress.move_west then
            text2_pos.x = text2_pos.x - 1
        elseif keypress.move_east then
            text2_pos.x = text2_pos.x + 1
        end
    end

    nyq.update()
    nyq.delay(10)
end
