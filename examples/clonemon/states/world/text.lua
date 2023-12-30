return State:new(
    function()
        nyq.keypress.trigger = true;
        if (nyq.keypress.select) then
            if ui.text_done then
                -- temps.state = states.world.main
                state = states.world.main
                ui.text_pos = 0
                ui.text_done = false
                ui.text_end = true
            elseif (ui.text_pos < ui.text_max) then
                ui.text_pos = ui.text_pos + 1
            end

        end
end)
