-- Event System
local class = require('class')
local table = table
local ipairs = ipairs
local type = type

local Event = class()

function Event:construct()
    self.delegates = {}
end

function Event:bind(a, b)
    if b == nil then
        table.insert(self.delegates, a)
    else
        table.insert(self.delegates, {table = a, func = b})
    end
end

function Event:unbind(a, b)
    if b == nil then
        for i=#self.delegates,1,-1 do
            if self.delegates[i] == a then
                table.remove(self.delegates, i)
            end
        end
    else
        for i=#self.delegates,1,-1 do
            local t = self.delegates[i]
            if type(t) == 'table' then
                if t.table == a and t.func == b then
                    table.remove(self.delegates, i)
                end
            end
        end
    end
end

function Event:clear()
    self.delegates = {}
end

function Event:call(...)
    for _,v in ipairs(self.delegates) do
        if type(v) == 'function' then
            v(...)
        else
            v.func(v.table, ...)
        end
    end
end

return Event