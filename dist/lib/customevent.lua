-- Event System
local class = require('class')
local table = table
local ipairs = ipairs

local CEvent = class()

function CEvent:construct()
    self.delegates = {}
end

function CEvent:bind(func)
    table.insert(self.delegates, func)
end

function CEvent:unbind(func)
    for k,v in ipairs(self.delegates) do
        if v == func then
            table.remove(self.delegates, k)
        end
    end
end

function CEvent:clear()
    self.delegates = {}
end

function CEvent:call(...)
    for _,v in ipairs(self.delegates) do
        v(...)
    end
end

return CEvent