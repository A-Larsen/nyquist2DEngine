table.Reverse = function(a)
    local b = {}
    for i = 1, #a do
        local j = #a - (i - 1)
        b[j] = {}
        b[j].id = i
        b[j].text = a[i]
    end
    return b
end
