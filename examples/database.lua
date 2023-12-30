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
-- nyq.database.init("test.db", schema);
nyq.database.init("test.db", {
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
});
-- nyq.database.insert("items", {type = 0, weight = 120, name = "pillow"})
-- nyq.database.insert("items", {type = 0, weight = 120, name = "frizbee"})
-- nyq.database.insert("items", {type = 1, weight = 120, name = "frizbee"})
-- nyq.database.insert("people", {name = "joe", age = 22})
-- nyq.database.insert("inventory", {person_id = 1, item_id = 1})
-- nyq.database.insert("items", {type = 2, weight = 1, name = "pill"})

print("---------------")
print("people")
print("---------------")
allPeople = nyq.database.getAll("people")

print("size: " .. #allPeople .. "\n")

for idx,table in pairs(allPeople) do
    print(idx)
    print('------')
    for k, v in pairs(table) do
        print(k, v)
    end
    print("\n")
end

print("---------------")
print("items")
print("---------------")
allItems = nyq.database.getAll("items")

print("size: " .. #allItems .. "\n")

for idx,table in pairs(allItems) do
    print(idx)
    print('------')
    for k, v in pairs(table) do
        print(k, v)
    end
    print("\n")
end

print("---------------")
print("inventory")
print("---------------")
allInventory = nyq.database.getAll("inventory")

print("size: " .. #allInventory .. "\n")

for idx,table in pairs(allInventory) do
    print(idx)
    print('------')
    for k, v in pairs(table) do
        print(k, v)
    end
    print("\n")
end

print(nyq.database.uuid())

