local test = require('test')
local Vector2 = Vector2

test.suite('Vector2 Library')

test.test('creation', function()
    local v = Vector2.new()
    test.assert_equal(#v, 2)
    test.expect_equal(v:get(1), 0)
    test.expect_equal(v:get(2), 0)

    v = Vector2.new(1, 2)
    test.assert_equal(#v, 2)
    test.expect_equal(v:get(1), 1)
    test.expect_equal(v:get(2), 2)

    v = Vector2.new{1, 2}
    test.assert_equal(#v, 2)
    test.expect_equal(v:get(1), 1)
    test.expect_equal(v:get(2), 2)

    test.expect_error(Vector2.new, 1, 2, 3, 4, 5, 6)
end)

test.test('equality', function()
    local a = Vector2.new(3, 4)
    local b = Vector2.new(5, 6)
    test.assert_not_equal(a, b)

    b = Vector2.new(3, 4)
    test.assert_equal(a, b)
end)

test.test('table as parameter', function()
    test.assert_equal(Vector2.get({1, 2}, 1), 1)
end)

test.test('table', function()
    local v = Vector2.new{1, 2 }
    local t = v:table()
    test.assert_equal(#t, 2)
    test.expect_equal(t[1], 1)
    test.expect_equal(t[2], 2)
end)

test.test('set', function()
    local v = Vector2.new()
    v:set(1, 1)
    v:set(2, 2)
    test.expect_equal(v:get(1), 1)
    test.expect_equal(v:get(2), 2)
end)

test.test('setAll', function()
    local v = Vector2.new()
    v:setAll(1, 2)
    test.expect_equal(v:get(1), 1)
    test.expect_equal(v:get(2), 2)
end)

test.test('dot', function()
    local a = Vector2.new(1, 1)
    local b = Vector2.new(2, 2)
    test.assert_equal(a:dot(b), 4)
end)

test.test('distance', function()
    local a = Vector2.new(1, 1)
    local b = Vector2.new(3, 1)
    test.assert_equal(a:distance(b), 2)
end)

test.test('length', function()
    local v = Vector2.new(3, 0)
    test.assert_equal(v:length(), 3)
end)

test.test('apply', function()
    local a = Vector2.new(1, 2)
    local b = a:apply(function(v, i) return v + i end)
    test.expect_equal(b:get(1), 2)
    test.expect_equal(b:get(2), 4)
end)

test.test('normalize', function()
    local a = Vector2.new(1, 2)
    local b = a:normalize()
    test.expect_num_equal(b:get(1), 1 / math.sqrt(5), 0.0000001)
    test.expect_num_equal(b:get(2), 2 / math.sqrt(5), 0.0000001)
end)

test.test('reflect', function()
    local a = Vector2.new(2, 3):normalize()
    local b = Vector2.new(1, 2):normalize()
    local result = a:reflect(b)
    test.expect_num_equal(result:get(1), -0.3328201174736, 0.0000001)
    test.expect_num_equal(result:get(2), -0.94299030303955, 0.0000001)
end)

test.test('refract', function()
    local a = Vector2.new(2, 3):normalize()
    local b = Vector2.new(1, 2):normalize()
    local result = a:refract(b, 0.75)
    test.expect_num_equal(result:get(1), -0.36206924915314, 0.0000001)
    test.expect_num_equal(result:get(2), -0.93215107917786, 0.0000001)
end)

test.test('table length', function()
    local a = Vector2.new(1, 2)
    test.expect_equal(#a, 2)
end)

test.test('unary minus', function()
    local a = Vector2.new(1, 2)
    a = -a
    test.expect_equal(a:get(1), -1)
    test.expect_equal(a:get(2), -2)
end)