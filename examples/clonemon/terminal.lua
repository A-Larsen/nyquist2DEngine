return  {
    commands = {
        setState = function(s)
            if s then
                -- temps.state = s
                state = s
                nyq.terminal.message("setting state")
            else
                nyq.terminal.error("could not set state")
            end
        end,
        start = function()
            states.start.main:reset()
            -- temps.state = states.start.main
            state = states.start.main
            temps.ui = uis.start
            -- nyq.audio.play("intro")
        end,
        world = function()
            states.world.main:reset()
            local menu_items = {
                [1] = { name = "Pokedex", },
                [2] = { name = "Pokemon", },
                [3] = { name = "bag", },
                [4] = { name = "Red", },
                [5] = { name = "Save", },
                [6] = { name = "Option", },
                [7] = { name = "Exit", }
            }
            params = {
                menu_items = menu_items,
                world_id = 2
            }
            -- temps.state = states.world
            -- temps.state = states.world.text
            -- temps.state = states.world.yesno
            state = states.world.option
            temps.ui = uis.world
            -- nyq.audio.play("world")
        end,
        battle = function()
            -- states.battle.intro:reset()
            params = {
                -- entry = "test",
                -- moves = moves.charzard,
                rivals_pokemon = {
                    [1] = {
                        name = "meowth",
                    },
                },
                my_pokemon = {
                    name = "charzard",
                    moves = moves.charzard,
                    level = 100,
                },
                player = {
                    name = "nyquist"
                }
            }
            uis.world.paused = false
            -- state = states.battle.narration
            -- state = states.battle.moveSelect
            -- state = states.battle.moveSelect
            -- state = states.battle.doSelect
            state = states.battle.enterBattle
            -- state = states.battle.intro
            temps.ui = uis.battle
        end,
        enter_text = function()
            params = {
                character_limit = 18,
                title = "enter name: ",
                sprite = "axolotl.png"
            }
            states.enter_text.main:reset()
            -- temps.state = states.enter_text.main
            state = states.enter_text.main
            temps.ui = uis.enter_text
        end,
        choice_select = function()
            params = {
                sprite = "choice_select_test_one.png",
                count = 2
                -- sprite = "choice_select_test_two.png",
                -- count = 3
            }
            states.choice_select.main:reset()
            -- temps.state = states.choice_select.main
            state = states.choice_select.main
            temps.ui = uis.choice_select
        end,
        bag = function()
            params = {
                items = {
                    [1] = {
                        name = "book",
                        description = "can read it",
                        image = "",
                        quantity = 1
                    },
                    [2] = {
                        name = "cup",
                        description = "drink things",
                        image = "",
                        quantity = 2
                    },
                    [3] = {
                        name = "pencil",
                        description = "write things",
                        image = "",
                        quantity = 3
                    }
                },
                berries = {
                    [1] = {
                        name = "blueberry",
                        description = "improves stats",
                        image = "",
                        quantity = 1
                    },
                    [2] = {
                        name = "raseberry",
                        description = "raises defense stat",
                        image = "",
                        quantity = 2
                    },
                    [3] = {
                        name = "blackberry",
                        description = "clears status effects",
                        image = "",
                        quantity = 3
                    }
                },
                cloneballs = {
                    [1] = {
                        name = "ULTRA BALL",
                        description = "A better Ball with a higher catch rate than a Great Ball",
                        image = "",
                        quantity = 1
                    },
                    [2] = {
                        name = "NET BALL",
                        description = "A BALL that works on WATER- and BUG- type Clonemon",
                        image = "",
                        quantity = 2
                    },
                }
            }
            states.bag.main:reset()
            -- temps.state = states.bag.main
            state = states.bag.main
            temps.ui = uis.bag
        end,
        party = function()
            states.party.main:reset()
            params = {
                my_clonemon = my_clonemon
            }
            state = states.party.main
            temps.ui = uis.party
        end
    }
}
