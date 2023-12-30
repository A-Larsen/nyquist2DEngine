return State:new(
    function()
        if (nyq.keypress.select) then
            -- Push_state()
            -- ui.clonemon_selected = ui.clonemon_switch
            -- ui.clonemon_switch = 1
            -- state = states.party.doWhat
            local temp = my_clonemon[ui.clonemon_selected]
            my_clonemon[ui.clonemon_selected] = my_clonemon[ui.clonemon_switch]
            my_clonemon[ui.clonemon_switch] = temp
            params.my_clonemon = my_clonemon
            ui.selected = ui.clonemon_switch
            -- temps.ui = uis.party
            -- state = states.party.main
            Pop_state()

        elseif (nyq.keypress.back) then
            -- terminal.commands.world()
            Pop_state()
        elseif (nyq.keypress.move_right) then
            if (ui.clonemon_switch + 1 <= 6) then
                ui.clonemon_switch = ui.clonemon_switch + 1
            end
        elseif (nyq.keypress.move_left) then
            if (ui.clonemon_switch - 1 > 0) then
                ui.clonemon_switch = ui.clonemon_switch - 1
            end
        elseif (nyq.keypress.move_down) then
            if (ui.clonemon_switch + 2 <= 6) then
                ui.clonemon_switch = ui.clonemon_switch + 2
            end
        elseif (nyq.keypress.move_up) then
            if (ui.clonemon_switch - 2 > 0) then
                ui.clonemon_switch = ui.clonemon_switch - 2
            end
        end

    end

)
