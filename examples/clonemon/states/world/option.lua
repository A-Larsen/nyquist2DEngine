return State:new(
    function()
        nyq.keypress.trigger = true;
        if (nyq.keypress.select) then
            -- temps.state = states.world.text
            state = states.world.text
            ui.text_pos  = ui.text_pos + 1
        elseif (nyq.keypress.move_down) and ui.selected < ui.selection_max then
            ui.selected = ui.selected + 1
        elseif (nyq.keypress.move_up)  and ui.selected  > 1 then
            ui.selected = ui.selected - 1
        end
end)
