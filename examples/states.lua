Stack = {
    states = {}
}

function Stack:push(func)
    Stack.states[#Stack.states + 1] = func
end

function Stack:next()
    func = table.remove(Stack.states, #Stack.states)
    if func then func() end
end

--- example bellow

-- NOTE: In a project all the states would have their own (almost infinite) loops
function state_world()
    print("state world")
end

function state_startMenu()
    print("state start menu")
end

function state_shop()
    print("state shop")
end

Stack:push(state_world)
Stack:push(state_startMenu)
Stack:push(state_shop)

Stack:next()
Stack:next()
Stack:next()
-- doesn't call this because there are no more elements in the stack
Stack:next()
