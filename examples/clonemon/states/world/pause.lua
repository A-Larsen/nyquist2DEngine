return State:new(
    function()
        nyq.keypress.trigger = true;
        if (nyq.keypress.select) then
            if ui.selected == 7 then
                nyq.exit()
            end
            if ui.selected == 3 then
                Push_state()
                terminal.commands.bag()
            end
            if ui.selected == 2 then
                Push_state()
                terminal.commands.enter_text()
            end
            if ui.selected == 4 then
                Push_state()
                terminal.commands.battle()
            end
        end
        if (nyq.keypress.pause) then
            -- temps.state = states.world.main
            state = states.world.main
        end
        if (nyq.keypress.move_down) then
            if (ui.selected + 1 <= ui.menu_size) then
                ui.selected = ui.selected + 1
            end
        elseif (nyq.keypress.move_up) then
            if (ui.selected - 1 > 0) then
                ui.selected = ui.selected - 1
            end
        end
    end
)
