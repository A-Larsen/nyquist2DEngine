return State:new(
    function()
        if (nyq.keypress.select) and ui.selected ~= 7 then
            Push_state()
            ui.clonemon_selected = ui.selected
            ui.selected = 1
            state = states.party.doWhat
        elseif (nyq.keypress.select) and ui.selected == 7 then
            Pop_state()
        elseif (nyq.keypress.back) then
            -- terminal.commands.world()
            Pop_state()
        elseif (nyq.keypress.move_right) then
            if ui.selected == 6 then
                ui.selected = 7
            elseif (ui.selected + 1 <= 6) then
                ui.selected = ui.selected + 1
            end
        elseif (nyq.keypress.move_left) then
            if (ui.selected - 1 > 0) then
                ui.selected = ui.selected - 1
            end
        elseif (nyq.keypress.move_down) then
            if ui.selected == 5 or ui.selected == 6 then
                ui.selected = 7
            elseif (ui.selected + 2 <= 6) then
                ui.selected = ui.selected + 2
            end
        elseif (nyq.keypress.move_up) then
            if ui.selected == 7 then
                ui.selected = 6
            elseif (ui.selected - 2 > 0) then
                ui.selected = ui.selected - 2
            end
        end

    end

)
