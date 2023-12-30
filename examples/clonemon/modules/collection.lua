local Collection = {}

function Collection:new()
    local o = {}
    o.metatable = {
        __index = {

            collection = {},
            elapsed_time = 0,
            addedNew = false,
            size = 0,

            push = function(self, st)
                self.collection[#self.collection + 1] = st
                self.addedNew = true
                self.size = #self.collection
            end,

            pop = function(self)
                local state = table.remove(self.collection, #self.collection)
                if state then 
                    state.first_call = true
                    self.addedNew = false
                    self.size = #self.collection - 1
                end
                return state
            end
        },
    }
    setmetatable(o, o.metatable)
    return o
end

return Collection
