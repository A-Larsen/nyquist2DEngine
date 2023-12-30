string.WordWrap = function (text, character_length)
    local str = text
    local spaces = {}
    local i = 1
    local lines = {}

    local found =  nil
    local total = 0
    repeat
        found = string.find(str, '%s')
        if found then
            total = total + found
            spaces[i] = total
            str = string.sub(str, found + 1)
        end
        i = i + 1
    until not found

    local start = 1
    found = 0
    i = 1
    for k, v in pairs(spaces) do
        if v > start + character_length then
            found = spaces[k - 1]
            local line = string.sub(text, start, found)
            if string.len(line) > 0 then
                lines[i] = line
                i = i + 1
            end
            start =  found + 1
        end
    end
    local line = string.sub(text, start)
    if string.len(line) > 0 then
        local match = find.reverse(line, '%s')
        if string.len(line) > character_length then
            if match then
                lines[i] = string.sub(line, 1, match[1])
                lines[i + 1] = string.sub(line, match[1] + 1)
            end
        else
            lines[i] = line
        end
    end
    -- if not lines then return {text} end

    return lines
end
