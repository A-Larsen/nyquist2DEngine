nyq.init("image", {x = 400, y = 100}, {w = 240, h = 160}, {scale = 3})

nyq.images.init()

image_pos = {x = 50, y = 50}

image_id = nyq.images.create("c++.png", image_pos, {w = 100, h = 100})

nyq.images.rotate(image_id, 45)
nyq.images.update(image_id, image_pos)

nyq.update()

nyq.delay(1000)
