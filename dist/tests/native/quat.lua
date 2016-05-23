local test = require('test')
local Quat = Quat
local math = math
local Vector2 = Vector2
local Vector3 = Vector3
local Vector4 = Vector4
local Matrix3x3 = Matrix3x3
local Matrix4x4 = Matrix4x4

test.suite('Quaternion Library')

test.test('creation', function()
    local q = Quat.new()
    test.assert_equal(#q, 4)
    test.expect_equal(q:get(1), 0)
    test.expect_equal(q:get(2), 0)
    test.expect_equal(q:get(3), 0)
    test.expect_equal(q:get(4), 1)

    q = Quat.new(1, 2, 3, 4)
    test.assert_equal(#q, 4)
    test.expect_equal(q:get(1), 2)
    test.expect_equal(q:get(2), 3)
    test.expect_equal(q:get(3), 4)
    test.expect_equal(q:get(4), 1)

    q = Quat.new{1, 2, 3}
    test.assert_equal(#q, 4)
    test.expect_num_equal(q:get(1), -0.718287, 0.000001)
    test.expect_num_equal(q:get(2), 0.310622, 0.000001)
    test.expect_num_equal(q:get(3), 0.444435, 0.000001)
    test.expect_num_equal(q:get(4), 0.435953, 0.000001)

    local c = Quat.new(q)
    test.assert_equal(#q, 4)
    test.expect_num_equal(q:get(1), -0.718287, 0.000001)
    test.expect_num_equal(q:get(2), 0.310622, 0.000001)
    test.expect_num_equal(q:get(3), 0.444435, 0.000001)
    test.expect_num_equal(q:get(4), 0.435953, 0.000001)

    q = Quat.new({1, 0, 0}, {0, 1, 0})
    test.assert_equal(#q, 4)
    test.expect_num_equal(q:get(1), 0, 0.000001)
    test.expect_num_equal(q:get(2), 0, 0.000001)
    test.expect_num_equal(q:get(3), 0.707107, 0.000001)
    test.expect_num_equal(q:get(4), 0.707107, 0.000001)

    test.expect_error(Quat.new, 1, 2, 3, 4, 5, 6)
end)

test.test('equality', function()
    local a = Quat.new{1, 2, 3}
    local b = Quat.new{4, 5, 6}
    test.assert_not_equal(a, b)

    b = Quat.new{1, 2, 3}
    test.assert_equal(a, b)
end)

test.test('table', function()
    local q = Quat.new{1, 2, 3}
    local t = q:table()
    test.assert_equal(#t, 4)
    test.expect_num_equal(t[1], -0.718287, 0.000001)
    test.expect_num_equal(t[2], 0.310622, 0.000001)
    test.expect_num_equal(t[3], 0.444435, 0.000001)
    test.expect_num_equal(t[4], 0.435953, 0.000001)
end)

test.test('set', function()
    local q = Quat.new()
    q:set(1, 1)
    q:set(2, 2)
    q:set(3, 3)
    q:set(4, 4)
    test.expect_equal(q:get(1), 1)
    test.expect_equal(q:get(2), 2)
    test.expect_equal(q:get(3), 3)
    test.expect_equal(q:get(4), 4)
end)

test.test('setAll', function()
    local q = Quat.new()
    q:setAll(1, 2, 3, 4)
    test.expect_equal(q:get(1), 1)
    test.expect_equal(q:get(2), 2)
    test.expect_equal(q:get(3), 3)
    test.expect_equal(q:get(4), 4)
end)

test.test('conjugate', function()
    local q = Quat.new{1, 2, 3}
    q = q:conjugate()
    test.expect_num_equal(q:get(1), 0.718287, 0.000001)
    test.expect_num_equal(q:get(2), -0.310622, 0.000001)
    test.expect_num_equal(q:get(3), -0.444435, 0.000001)
    test.expect_num_equal(q:get(4), 0.435953, 0.000001)
end)

test.test('cross', function()
    local q = Quat.cross(Quat.new{1, 2, 3}, Quat.new{4, 5, 6})
    test.expect_num_equal(q:get(1), 0.629709, 0.000001)
    test.expect_num_equal(q:get(2), 0.740911, 0.000001)
    test.expect_num_equal(q:get(3), -0.195374, 0.000001)
    test.expect_num_equal(q:get(4), 0.127854, 0.000001)
end)

test.test('dot', function()
    local d = Quat.dot(Quat.new{1, 2, 3}, Quat.new{4, 5, 6})
    test.assert_num_equal(d, -0.348674, 0.000001)
end)

test.test('inverse', function()
    local q = Quat.new{1, 2, 3}
    q = q:inverse()
    test.expect_num_equal(q:get(1), 0.718287, 0.000001)
    test.expect_num_equal(q:get(2), -0.310622, 0.000001)
    test.expect_num_equal(q:get(3), -0.444435, 0.000001)
    test.expect_num_equal(q:get(4), 0.435953, 0.000001)
end)

test.test('length', function()
    local q = Quat.new{1, 2, 3}
    test.assert_num_equal(q:length(), 1, 0.000001)
end)

test.test('apply', function()
    local a = Quat.new(4, 1, 2, 3)
    local b = a:apply(function (v, i) return v + i end)
    test.expect_equal(b:get(1), 2)
    test.expect_equal(b:get(2), 4)
    test.expect_equal(b:get(3), 6)
    test.expect_equal(b:get(4), 8)
end)

test.test('mat3', function()
    local q = Quat.new{1, 2, 3}
    local m = q:mat3()
    local expected = {0.411982, -0.058727, -0.909297, -0.833738, -0.426917, -0.350175, -0.36763, 0.902382, -0.224845 }
    local actual = m:table()
    for i=1,#expected do
        test.expect_num_equal(expected[i], actual[i], 0.000001)
    end
end)

test.test('mat4', function()
    local q = Quat.new{1, 2, 3}
    local m = q:mat4()
    local expected = {0.411982, -0.058727, -0.909297, 0, -0.833738, -0.426917, -0.350175, 0, -0.367630, 0.902382, -0.224845, 0, 0, 0, 0, 1 }
    local actual = m:table()
    for i=1,#expected do
        test.expect_num_equal(expected[i], actual[i], 0.000001)
    end
end)

test.test('mix', function()
    local q = Quat.mix(Quat.new{1, 2, 3}, Quat.new{3, 2, 1}, 0.5)
    test.expect_num_equal(q:get(1), -0.24054, 0.000001)
    test.expect_num_equal(q:get(2), 0.545675, 0.000001)
    test.expect_num_equal(q:get(3), -0.24054, 0.000001)
    test.expect_num_equal(q:get(4), 0.765845, 0.000001)
end)

test.test('normalize', function()
    local q = Quat.new(1, 2, 3, 4)
    q = q:normalize()
    test.expect_num_equal(q:get(1), 0.365148, 0.000001)
    test.expect_num_equal(q:get(2), 0.547723, 0.000001)
    test.expect_num_equal(q:get(3), 0.730297, 0.000001)
    test.expect_num_equal(q:get(4), 0.182574, 0.000001)
end)

test.test('rotate', function()
    local q = Quat.rotate(Quat.new{1, 2, 3}, math.radians(45), {1, 0, 0})
    test.expect_num_equal(q:get(1), -0.496779, 0.000001)
    test.expect_num_equal(q:get(2), 0.457056, 0.000001)
    test.expect_num_equal(q:get(3), 0.291734, 0.000001)
    test.expect_num_equal(q:get(4), 0.677644, 0.000001)
end)

test.test('operations', function()
    local q = Quat.new{0, 1, 0}
    local v3 = Vector3.new(1, 2, 3)
    local v4 = Vector4.new(1, 2, 3, 4)

    local r = q + q
    test.expect_num_equal(r:get(1), 0, 0.000001)
    test.expect_num_equal(r:get(2), 0.958851, 0.000001)
    test.expect_num_equal(r:get(3), 0, 0.000001)
    test.expect_num_equal(r:get(4), 1.755165, 0.000001)

    r = q * q
    test.expect_num_equal(r:get(1), 0, 0.000001)
    test.expect_num_equal(r:get(2), 0.841471, 0.000001)
    test.expect_num_equal(r:get(3), 0, 0.000001)
    test.expect_num_equal(r:get(4), 0.540302, 0.000001)

    test.expect_error(function()
        r = q * Vector2.new(1, 5)
    end)

    r = v3 * q
    test.expect_num_equal(r:get(1), -1.984111, 0.000001)
    test.expect_num_equal(r:get(2), 2, 0.000001)
    test.expect_num_equal(r:get(3), 2.462378, 0.000001)

    r = q * v3
    test.expect_num_equal(r:get(1), 3.064715, 0.000001)
    test.expect_num_equal(r:get(2), 2, 0.000001)
    test.expect_num_equal(r:get(3), 0.779436, 0.000001)

    r = v4 * q
    test.expect_num_equal(r:get(1), -1.984111, 0.000001)
    test.expect_num_equal(r:get(2), 2, 0.000001)
    test.expect_num_equal(r:get(3), 2.462378, 0.000001)
    test.expect_num_equal(r:get(4), 4, 0.000001)

    r = q * v4
    test.expect_num_equal(r:get(1), 3.064715, 0.000001)
    test.expect_num_equal(r:get(2), 2, 0.000001)
    test.expect_num_equal(r:get(3), 0.779436, 0.000001)
    test.expect_num_equal(r:get(4), 4, 0.000001)

    r = -q
    test.expect_num_equal(r:get(1), 0, 0.000001)
    test.expect_num_equal(r:get(2), -0.479426, 0.000001)
    test.expect_num_equal(r:get(3), 0, 0.000001)
    test.expect_num_equal(r:get(4), -0.877583, 0.000001)
end)