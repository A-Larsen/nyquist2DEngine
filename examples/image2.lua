nyq.init("image", {x = 400, y = 100}, {w = 240, h = 160})

image_pos = {x = 50, y = 50}

-- keypress = nyq.events.init()

image_id = nyq.images.create("c++.png", image_pos, {w = 100, h = 100})

function drawLoop()
    nyq.images.rotate(image_id, 45)
    nyq.images.update(image_id, image_pos)
end

