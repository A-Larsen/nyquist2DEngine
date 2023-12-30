nyq.init("audio", {x = 400, y = 100}, {w = 240, h = 160}, {
    scale = 3,
    terminal = {
        bg_color = {r = 0, g = 204, b = 0, a = 150},
    }
})

nyq.audio.init({
    kick = "meta19.wav",
    hihat = "hihat.wav",
    snare = "snare.wav",
})

keypress = nyq.events.init({
    kick = "J",
    snare = "K",
    hihat = "L",
})

function eventLoop()
    nyq.events.update(keypress)
    if nyq.events.trigger(keypress.kick) then

        if nyq.waiting.isFinished(uuid1) then
            nyq.audio.play("kick")
            uuid1 = nyq.waiting.new(1000)
            print(uuid1)
        end


    elseif nyq.events.trigger(keypress.snare) then

        if nyq.waiting.isFinished(uuid2) then
            nyq.audio.play("snare")
            uuid2 = nyq.waiting.new(5000)
            print(uuid2)
        end

    elseif nyq.events.trigger(keypress.hihat) then
        nyq.audio.play("hihat")
    end


end

function gameLoop()
    nyq.clear({r = 10, g = 0, b = 0, a = 0})
end
