return State:new(
    function()
        local canInput = not uis.enter_text.full and string.len(uis.enter_text.string) < uis.enter_text.character_limit

        if controller == "gamepad" then
            if nyq.keypress.move_right  and ui.cursor <= 31 then
                ui.cursor = ui.cursor + 1
            elseif nyq.keypress.move_left  and ui.cursor > 0 then
                if ui.cursor == 32 then
                    ui.cursor = ui.cursor - 2
                else
                    ui.cursor = ui.cursor - 1
                end
            elseif nyq.keypress.move_down and ui.cursor < 22 then
                ui.cursor = ui.cursor + 11
            elseif nyq.keypress.move_up and ui.cursor >= 11 then
                ui.cursor = ui.cursor - 11
            elseif nyq.keypress.back then
                ui.string = string.sub(ui.string, 1, string.len(ui.string) - 1)
            elseif nyq.keypress.bag then
                ui.isUpper = not ui.isUpper
            elseif nyq.keypress.hotkey then
                Pop_state()
                -- terminal.commands.world()
            end

            local letter = ui.letter_selection[ui.cursor + 1]
            if nyq.keypress.select then
                if canInput and letter:match("[%a ]") then
                    if ui.isUpper then
                        ui.string = ui.string .. string.upper(letter)
                    else
                        ui.string = ui.string .. letter
                    end
                elseif letter == ui.key_backspace then
                    ui.string = string.sub(ui.string, 1, string.len(ui.string) - 1)
                elseif letter == ui.key_change_case then
                    ui.isUpper = not ui.isUpper
                elseif letter == ui.key_done then
                    -- terminal.commands.world()
                    Pop_state()
                end
            end
            -- print(ui.cursor)
        else
            if nyq.keypress.select then
                -- terminal.commands.world()
                Pop_state()
            end
        end

        nyq.keypress.trigger = true;

        if nyq.key then
            if nyq.key == '\b' then
                if string.len(uis.enter_text.string) > 0 then
                    uis.enter_text.string = string.sub(uis.enter_text.string, 1, string.len(uis.enter_text.string) - 1)
                end
                if uis.enter_text.full then
                    uis.enter_text.full = false
                end
            else
                if canInput then
                    uis.enter_text.string = uis.enter_text.string .. nyq.key
                end
            end
        end
    end
)
