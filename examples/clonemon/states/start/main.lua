return State:new(
    function ()
        nyq.keypress.trigger = true;
        if nyq.keypress.select then
            -- terminal.commands.world()
            players = nyq.database.getAll("players")
            print("loading all players: \n")
            for k, v in pairs(players) do
                for k2, v2 in pairs(v) do
                    print(k2, v2)
                end
                print()
            end
            temps.ui = uis.loadsave
            state = states.loadsave.main
        elseif nyq.keypress.change_moves1 then
            -- pretend like this is a new file
            -- nyq.database.insert("moves", {
            --     name = "NONE",
            --     pp = 0,
            --     power = 0
            -- })
            -- nyq.database.insert("moves", {
            --     name = "kick",
            --     pp = 200,
            --     power = 20
            -- })
            -- nyq.database.insert("pokemon", {
            --     name = "charzard",
            --     type = 1,
            --     gender = 0,
            --     move_one_id = 1,
            --     move_two_id = 1,

            -- })
            -- nyq.database.insert("players", {
            --     name = "ash",
            --     uuid = nyq.database.uuid(),
            --     age = 33,
            --     pokemon_id = 1
            -- })
            nyq.database.insert("people", {
                name = "blue",
                age = 500,
                level = 10,
                spritesheet = "sup.png"
            })
            nyq.database.insert("players", {
                uuid = nyq.database.uuid(),
                person_id = 2
            })
        end
    end
)
