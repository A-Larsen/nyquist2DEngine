local time = {}

function time.start()
    time.start = os.clock()
end

function time.get()
    time.start = os.clock()
end

return time
