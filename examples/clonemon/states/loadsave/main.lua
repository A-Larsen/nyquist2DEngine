return State:new(
    function()
        if nyq.keypress.select then
            if #players == 0 then
                print("go to player creation")
            elseif ui.selected == 1 then
                player = nyq.database.getFromUUID("players", players[ui.selected2].uuid)
                print("loading player:")
                for k, v in pairs(player) do 
                    print(k, v)
                end
            elseif ui.selected == 2 then
                print("go to player creation")
            end
            local menu_items = {
                [1] = { name = "Pokedex", },
                [2] = { name = "Pokemon", },
                [3] = { name = "bag", },
                [4] = { name = "Red", },
                [5] = { name = "Save", },
                [6] = { name = "Option", },
                [7] = { name = "Exit", }
            }
            params = {
                menu_items = menu_items,
                world_id = 2
            }
            temps.ui = uis.world
            state = states.world.main
        end
        if #players > 0 then
            if (nyq.keypress.move_down) then
                if (ui.selected + 1 <= ui.menu_size) then
                    ui.selected = ui.selected + 1
                end
            elseif (nyq.keypress.move_up) then
                if (ui.selected - 1 > 0) then
                    ui.selected = ui.selected - 1
                end
            elseif (nyq.keypress.move_right) then
                if (ui.selected2 + 1 <= #players) then
                    ui.selected2 = ui.selected2 + 1
                end
            elseif (nyq.keypress.move_left) then
                if (ui.selected2 - 1 > 0) then
                    ui.selected2 = ui.selected2 - 1
                end
            end
        end
    end
)

