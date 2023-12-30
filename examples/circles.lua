nyq.init("circles", {x = 400, y = 100}, {w = 800, h = 800})



keypress = nyq.events.init({
    move_north = "W",
    move_east = "D",
    move_south = "S",
    move_west = "A",
    rotate_right = "L",
    rotate_left = "J"
})

circlePos = {x = 20, y = 20}
color1 = {r = 255, g = 0, b = 0, a = 0}
color2 = {r = 0, g = 255, b = 0, a = 0}
while not nyq.quit do
    nyq.clear({r = 0, g = 0, b = 0, a = 0})
    nyq.events.update(keypress)

    if keypress.move_north then
        circlePos.y = circlePos.y - 1
    elseif keypress.move_south then
        circlePos.y = circlePos.y + 1
    elseif keypress.move_east then
        circlePos.x = circlePos.x + 1
    elseif keypress.move_west then
        circlePos.x = circlePos.x - 1
    end
    nyq.drawCircle(circlePos, color1, 20)
    nyq.drawCircle(circlePos, color2, 30)
    nyq.update()
    nyq.delay(10)
end

