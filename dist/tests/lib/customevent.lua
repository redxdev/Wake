local test = require('test')
local CEvent = require('customevent')
local type = type

test.suite('Event Lua Library')

test.test('construct', function()
    local e = CEvent.new()
    test.assert_equal(type(e), 'table')
end)