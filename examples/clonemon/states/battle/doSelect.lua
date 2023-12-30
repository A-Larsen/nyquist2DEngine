return State:new(
    function()
        if (nyq.keypress.select) then
            -- terminal.commands.world()
            -- Pop_state()
                if selected == 1 then
                    Push_state()
                    state = states.battle.moveSelect
                elseif selected == 2 then
                    Push_state()
                    terminal.commands.bag()
                elseif selected == 3 then
                    Push_state()
                    terminal.commands.party()
                end
        elseif (nyq.keypress.back) then
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
        end

    end

)
