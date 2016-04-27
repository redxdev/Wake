local test = require('test')
local Vector3 = Vector3

test.suite('Vector3 Library')

test.test('creation', function()
    local v = Vector3.new()
    test.assert_equal(#v, 3)
    test.expect_equal(v:get(1), 0)
    test.expect_equal(v:get(2), 0)
    test.expect_equal(v:get(3), 0)

    v = Vector3.new(1, 2, 3)
    test.assert_equal(#v, 3)
    test.expect_equal(v:get(1), 1)
    test.expect_equal(v:get(2), 2)
    test.expect_equal(v:get(3), 3)

    v = Vector3.new{1, 2, 3}
    test.assert_equal(#v, 3)
    test.expect_equal(v:get(1), 1)
    test.expect_equal(v:get(2), 2)
    test.expect_equal(v:get(3), 3)
end)

test.test('equality', function()
    local a = Vector3.new(3, 4, 5)
    local b = Vector3.new(5, 6, 7)
    test.assert_not_equal(a, b)

    b = Vector3.new(3, 4, 5)
    test.assert_equal(a, b)
end)

test.test('table as parameter', function()
    test.assert_equal(Vector3.get({1, 2, 3}, 3), 3)
end)

test.test('table', function()
    local v = Vector3.new{1, 2, 3}
    local t = v:table()
    test.assert_equal(#t, 3)
    test.expect_equal(t[1], 1)
    test.expect_equal(t[2], 2)
    test.expect_equal(t[3], 3)
end)

test.test('set', function()
    local v = Vector3.new()
    v:set(1, 1)
    v:set(2, 2)
    v:set(3, 3)
    test.expect_equal(v:get(1), 1)
    test.expect_equal(v:get(2), 2)
    test.expect_equal(v:get(3), 3)
end)

test.test('setAll', function()
    local v = Vector3.new()
    v:setAll(1, 2, 3)
    test.expect_equal(v:get(1), 1)
    test.expect_equal(v:get(2), 2)
    test.expect_equal(v:get(3), 3)
end)

test.test('dot', function()
    local a = Vector3.new(1, 1, 1)
    local b = Vector3.new(2, 2, 2)
    test.assert_equal(a:dot(b), 6)
end)

test.test('distance', function()
    local a = Vector3.new(1, 1, 1)
    local b = Vector3.new(3, 1, 1)
    test.assert_equal(a:distance(b), 2)
end)

test.test('length', function()
    local v = Vector3.new(3, 0, 0)
    test.assert_equal(v:length(), 3)
end)

test.test('apply', function()
    local a = Vector3.new(1, 2, 3)
    local b = a:apply(function(v, i) return v + i end)
    test.expect_equal(b:get(1), 2)
    test.expect_equal(b:get(2), 4)
    test.expect_equal(b:get(3), 6)
end)

test.test('normalize', function()
    local a = Vector3.new(1, 2, 3)
    local b = a:normalize()
    test.expect_num_equal(b:get(1), 1 / math.sqrt(14), 0.0000001)
    test.expect_num_equal(b:get(2), math.sqrt(2 / 7), 0.0000001)
    test.expect_num_equal(b:get(3), 3 / math.sqrt(14), 0.0000001)
end)

test.test('reflect', function()
    local a = Vector3.new(2, 3, 4):normalize()
    local b = Vector3.new(1, 2, 3):normalize()
    local result = a:reflect(b)
    test.expect_num_equal(result:get(1), -0.15916737914085, 0.0000001)
    test.expect_num_equal(result:get(2), -0.50403010845184, 0.0000001)
    test.expect_num_equal(result:get(3), -0.84889274835587, 0.0000001)
end)

test.test('refract', function()
    local a = Vector3.new(2, 3, 4):normalize()
    local b = Vector3.new(1, 2, 3):normalize()
    local result = a:refract(b, 0.75)
    test.expect_num_equal(result:get(1), -0.18656432628632, 0.0000001)
    test.expect_num_equal(result:get(2), -0.51240015029907, 0.0000001)
    test.expect_num_equal(result:get(3), -0.83823597431183, 0.0000001)
end)

test.test('cross', function()
    local a = Vector3.new(1, 2, 3)
    local b = Vector3.new(3, 2, 1)
    local result = a:cross(b)
    test.expect_num_equal(result:get(1), -4)
    test.expect_num_equal(result:get(2), 8)
    test.expect_num_equal(result:get(3), -4)
end)

test.test('table length', function()
    local a = Vector3.new(1, 2, 3)
    test.expect_equal(#a, 3)
end)

test.test('unary minus', function()
    local a = Vector3.new(1, 2, 3)
    a = -a
    test.expect_equal(a:get(1), -1)
    test.expect_equal(a:get(2), -2)
    test.expect_equal(a:get(3), -3)
end)