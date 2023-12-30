return State:new(
    function()
        if (nyq.keypress.select) then
            -- Push_state()
            -- state = states.party.doWhat
            if ui.selected == 2 then
                ui.clonemon_switch = ui.clonemon_selected
                state = states.party.switch
            end
        elseif (nyq.keypress.back) then
            ui.selected = ui.clonemon_selected
            Pop_state()
        elseif (nyq.keypress.move_down)  and ui.selected < ui.selection_max then
            ui.selected = ui.selected + 1
        elseif (nyq.keypress.move_up) and ui.selected > 1 then
            ui.selected = ui.selected - 1
        end

    end

)
