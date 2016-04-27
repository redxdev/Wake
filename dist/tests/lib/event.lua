local test = require('test')
local Event = require('event')
local type = type

test.suite('Event Library')

test.test('construct', function()
    local e = Event.new()
    test.assert_equal(type(e), 'table')
end)

test.test('call', function()
    local e = Event.new()

    local a = 0
    local b = 0

    e:bind(function(x)
        a = x
    end)
    e:bind(function(x)
        b = x
    end)

    e:call(123)

    test.expect_equal(a, 123)
    test.expect_equal(b, 123)
end)

test.test('unbind', function()
    local e = Event.new()

    local a = 0
    local b = 0

    local aFunc = function(x) a = x end
    local bFunc = function(x) b = x end

    e:bind(aFunc)
    e:bind(bFunc)
    e:bind(bFunc)

    e:call(123)
    test.expect_equal(a, 123)
    test.expect_equal(b, 123)

    e:unbind(bFunc)

    e:call(321)
    test.expect_equal(a, 321)
    test.expect_equal(b, 123)

    e:clear()

    e:call(0)
    test.expect_equal(a, 321)
    test.expect_equal(b, 123)
end)