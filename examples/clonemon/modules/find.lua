local find = {}

function find.reverse(str, pattern)
    local reversed = string.reverse(str)
    local match = table.pack(string.find(reversed, pattern))
    if match[1] and match[2] then
        match[1] = string.len(str) - match[1] + 1
        match[2] = string.len(str) - match[2] + 1
        return match
    end
    return nil
end

return find
