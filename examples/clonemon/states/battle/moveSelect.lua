return State:new(
    function()
        if (nyq.keypress.back) then
            -- terminal.commands.world()
            Pop_state()
        elseif (nyq.keypress.move_right) then
            if (selected + 1 <= 4) then
                selected = selected + 1
            end
        elseif (nyq.keypress.move_left) then
            if (selected - 1 > 0) then
                selected = selected - 1
            end
        elseif (nyq.keypress.move_down) then
            if (selected + 2 <= 4) then
                selected = selected + 2
            end
        elseif (nyq.keypress.move_up) then
            if (selected - 2 > 0) then
                selected = selected - 2
            end
        elseif (nyq.keypress.change_moves1) then
            -- battle_ui:addMoves(moves["pikachu"])
        elseif (nyq.keypress.change_moves2) then
            -- battle_ui:addMoves(moves["charzard"])
        end

    end

)
