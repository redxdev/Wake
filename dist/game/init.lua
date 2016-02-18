print "Startup!"

local time = 10
local next = time - 1
engine.tick:bind(function(dt)
    time = time - dt
    if time < next then
        print(next)
        next = next - 1
    end

    if time < 0 then
        engine.stop()
    end
end)

engine.quit:bind(function()
    print "Quitting!"
end)