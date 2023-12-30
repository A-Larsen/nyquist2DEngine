return State:new(
    function()
        nyq.keypress.trigger = false;
        if (nyq.keypress.pause) then
            state = states.world.pause
            uis.world.paused = true
            -- world_ui:pauseToggle()
        end

            uis.world:collisionCheck()
            if nyq.keypress.move_north then
                uis.world:moveNorth()
            elseif nyq.keypress.move_east then
                uis.world:moveEast()
            elseif nyq.keypress.move_south then
                uis.world:moveSouth()
            elseif nyq.keypress.move_west then
                uis.world:moveWest()
            end
    end

)
