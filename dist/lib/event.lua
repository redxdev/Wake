-- Event System
local class = require('class')
local table = table
local ipairs = ipairs

local Event = class()

function Event:construct()
    self.delegates = {}
end

function Event:bind(func)
    table.insert(self.delegates, func)
end

function Event:unbind(func)
    for i=#self.delegates,1,-1 do
        if self.delegates[i] == func then
            table.remove(self.delegates, i)
        end
    end
end

function Event:clear()
    self.delegates = {}
end

function Event:call(...)
    for _,v in ipairs(self.delegates) do
        v(...)
    end
end

return Event