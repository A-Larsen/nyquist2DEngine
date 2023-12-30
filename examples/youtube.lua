schema = {
    items = {
        type = DB_INTEGER_UNIQUE,
        weight = DB_INTEGER_UNIQUE,
        name = DB_TEXT_UNIQUE
    },
    inventory = {
        person_id = "people",
        item_id = "items"
    },
    people = {
        name = DB_TEXT,
        age = DB_INTEGER,
    }
}
nyq.database.init("test.db", schema)
-- nyq.database.insert("items", {type = 1, weight = 3, name="pillow"})
-- nyq.database.insert("people", {name = "Joe", age=55})
-- nyq.database.insert("inventory", {person_id = 1, item_id = 1})

nyq.init("one", {x = 200, y = 200}, {w = 800, h = 800})

text_pos = {x = 0, y = 0}

text_id = nyq.text.create(
    "NotoSansMono-Regular.ttf", 50,
    "hello!", 
    {r = 255, g = 255, b = 255, a = 255},
    {r = 0, g = 0, b = 0, a = 0}
)


keypress = nyq.events.init({
    move_north = "W",
    move_east = "D",
    move_south = "S",
    move_west = "A",
})

image_pos = {x = 200, y = 200}
image_id = nyq.images.create("c++.png", image_pos, {w = 100, h = 100})

while not nyq.quit do
    nyq.events.update(keypress)
    nyq.clear({r = 0, g = 0, b = 0, a = 0})

    if keypress.move_north then
        image_pos.y = image_pos.y - 1
    elseif keypress.move_south then
        image_pos.y = image_pos.y + 1
    elseif keypress.move_east then
        image_pos.x = image_pos.x + 1
    elseif keypress.move_west then
        image_pos.x = image_pos.x - 1
    end

    nyq.text.update(text_id, text_pos)
    nyq.images.update(image_id, image_pos)

    nyq.update()
    nyq.delay(10)
end
