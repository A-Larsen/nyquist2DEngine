return State:new(
    function()
        if nyq.keypress.move_right and ui.selected < ui.max then
            ui.selected = ui.selected + 1
        elseif nyq.keypress.move_left and ui.selected > 1 then
            ui.selected = ui.selected - 1
        elseif nyq.keypress.select then
            print(ui.selected)
        end
    end
)
