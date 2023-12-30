return State:new(
    function()
        if nyq.keypress.back then
            Pop_state()
        end
        if nyq.keypress.move_right then
            ui.type_selected = (ui.type_selected % ui.type_section_max) + 1
            ui.selected = 1
        elseif nyq.keypress.move_left then
            ui.type_selected = ((ui.type_selected - 1) % ui.type_section_max)
            if ui.type_selected == 0 then
                ui.type_selected = ui.type_section_max
            end
            ui.selected = 1
        elseif nyq.keypress.move_down and ui.selected < ui.selection_max then
            ui.selected = ui.selected + 1
        elseif nyq.keypress.move_up and ui.selected > 1 then
            ui.selected = ui.selected - 1
        end
    end
)
