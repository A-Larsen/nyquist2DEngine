return State:new(function()
    -- battle_ui:pokemonEnterNarration("blue")
    if ui.narration.pages > 1 then
        if nyq.keypress.select and ui.narration.page_idx < ui.narration.pages then
            ui.narration.page_idx = ui.narration.page_idx + 1
        end
    else
        if nyq.keypress.select then
            state = states.battle.doSelect
        end
    end
end)
