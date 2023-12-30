local State = {}

function State:new(call)
    local o = {}
    o.metatable = {
        __index = {
            first_call = true,
            elapsed_time = 0,
            start_time = 0,
        },
        __call = function(table, key)
            if table.first_call then
                table.start_time = nyq.getTicks()
                table.first_call = false
            end
            call()
            -- table.elapsed_time = os.clock() - table.start_time
            table.elapsed_time = nyq.getTicks() - table.start_time
        end

    }
    o.reset = function()
        o.start_time = nyq.getTicks()
        o.first_call = true
        o.elapsed_time = 0

    end
    setmetatable(o, o.metatable)
    return o
end

return State
