nyq.init("audio", {x = 400, y = 100}, {w = 240, h = 160}, {
    keys = {
        kick = "J",
        snare = "K",
        hihat = "L",
    }
})

nyq.audio.init({
    kick = "meta19.wav",
    hihat = "hihat.wav",
    snare = "snare.wav",
})

function logicLoop()
    if nyq.events.trigger(nyq.keypress.kick) then
        nyq.audio.play("kick")
    elseif nyq.events.trigger(nyq.keypress.snare) then
        nyq.audio.play("snare")
    elseif nyq.events.trigger(nyq.keypress.hihat) then
        nyq.audio.play("hihat")
    end
end
