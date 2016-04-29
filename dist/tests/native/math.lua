local test = require('test')
local math = math
local Vector2 = Vector2
local Vector3 = Vector3
local Vector4 = Vector4

test.suite('Math Extension Library')

test.test('radians', function()
    test.expect_num_equal(math.radians(90), math.pi / 2)
end)

test.test('degrees', function()
    test.expect_num_equal(math.degrees(math.pi / 2), 90)
end)

test.test('frustum', function()
    local frustum = math.frustum(100, 200, 10, 20, 0.1, 1000)
    local expected = {0.002, 0, 0, 0, 0, 0.02, 0, 0, 3, 3, -1.0002, -1, 0, 0, -0.20002, 0 }
    local actual = frustum:table()
    for i=1,#expected do
        test.expect_num_equal(expected[i], actual[i], 0.000001)
    end
end)

test.test('lookAt', function()
    local mat = math.lookAt({4, 3, 3}, {0, 0, 0}, {0, 1, 0})
    local expected = {0.6, -0.411597, 0.685994, 0, 0, 0.857493, 0.514496, 0, -0.8, -0.308697, 0.514496, 0, 0, 0, -5.830952, 1 }
    local actual = mat:table()
    for i=1,#expected do
        test.expect_num_equal(expected[i], actual[i], 0.000001)
    end
end)

test.test('ortho', function()
    local ortho = math.ortho(0, 800, 0, 600)
    local expected = {0.0025, 0, 0, 0, 0, 0.003333, 0, 0, 0, 0, -1, 0, -1, -1, 0, 1}
    local actual = ortho:table()
    for i=1,#expected do
        test.expect_num_equal(expected[i], actual[i], 0.000001)
    end

    ortho = math.ortho(0, 800, 0, 600, 0.1, 1000)
    local expected = {0.0025, 0, 0, 0, 0, 0.003333, 0, 0, 0, 0, -0.002, 0, -1, -1, -1.0002, 1}
    local actual = ortho:table()
    for i=1,#expected do
        test.expect_num_equal(expected[i], actual[i], 0.000001)
    end
end)

test.test('perspective', function()
    local pers = math.perspective(math.radians(45), 4/3, 0.1, 1000)
    local expected = {1.81066, 0, 0, 0, 0, 2.414214, 0, 0, 0, 0, -1.0002, -1, 0, 0, -0.20002, 0}
    local actual = pers:table()
    for i=1,#expected do
        test.expect_num_equal(expected[i], actual[i], 0.000001)
    end

    pers = math.perspective(math.radians(45), 4/3, 0.1)
    expected = {1.81066, 0, 0, 0, 0, 2.414214, 0, 0, 0, 0, -1, -1, 0, 0, -0.2, 0}
    actual = pers:table()
    for i=1,#expected do
        test.expect_num_equal(expected[i], actual[i], 0.000001)
    end
end)

test.test('rotate', function()
    local rot = math.rotate(math.radians(90), {1, 0, 0})
    local expected = {1, 0, 0, 0, 0, 0, 1, 0, 0, -1, 0, 0 }
    local actual = rot:table()
    for i=1,#expected do
        test.expect_num_equal(expected[i], actual[i], 0.000001)
    end

    rot = math.rotate(Matrix4x4.new(), math.radians(90), {1, 0, 0})
    expected = {1, 0, 0, 0, 0, 0, 1, 0, 0, -1, 0, 0 }
    actual = rot:table()
    for i=1,#expected do
        test.expect_num_equal(expected[i], actual[i], 0.000001)
    end
end)

test.test('scale', function()
    local mat = math.scale({2, 3, 4})
    local expected = {2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4, 0, 0, 0, 0, 1}
    local actual = mat:table()
    for i=1,#expected do
        test.expect_num_equal(expected[i], actual[i], 0.000001)
    end

    mat = math.scale(Matrix4x4.new(), {2, 3, 4})
    expected = {2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4, 0, 0, 0, 0, 1}
    actual = mat:table()
    for i=1,#expected do
        test.expect_num_equal(expected[i], actual[i], 0.000001)
    end
end)

test.test('translate', function()
    local mat = math.translate({1, 2, 3})
    local expected = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 2, 3, 1}
    local actual = mat:table()
    for i=1,#expected do
        test.expect_num_equal(expected[i], actual[i], 0.000001)
    end

    mat = math.translate(Matrix4x4.new(), {1, 2, 3})
    expected = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 2, 3, 1}
    actual = mat:table()
    for i=1,#expected do
        test.expect_num_equal(expected[i], actual[i], 0.000001)
    end
end)

test.test('abs', function()
    test.expect_equal(math.abs(10), 10)
    test.expect_equal(math.abs(-10), 10)

    test.expect_equal(math.abs(Vector2.new(10, 20)), Vector2.new(10, 20))
    test.expect_equal(math.abs(Vector2.new(-10, -20)), Vector2.new(10, 20))

    test.expect_equal(math.abs(Vector3.new(10, 20, 30)), Vector3.new(10, 20, 30))
    test.expect_equal(math.abs(Vector3.new(-10, -20, -30)), Vector3.new(10, 20, 30))


    test.expect_equal(math.abs(Vector4.new(10, 20, 30, 40)), Vector4.new(10, 20, 30, 40))
    test.expect_equal(math.abs(Vector4.new(-10, -20, -30, -40)), Vector4.new(10, 20, 30, 40))
end)

test.test('ceil', function()
    test.expect_equal(math.ceil(1.234), 2)
    test.expect_equal(math.ceil(Vector2.new(1.2, 2.3)), Vector2.new(2, 3))
    test.expect_equal(math.ceil(Vector3.new(1.2, 2.3, 3.7)), Vector3.new(2, 3, 4))
    test.expect_equal(math.ceil(Vector4.new(1.2, 2.3, 3.7, 9.8)), Vector4.new(2, 3, 4, 10))
end)

test.test('clamp', function()
    test.expect_equal(math.clamp(1, 2, 6), 2)
    test.expect_equal(math.clamp(7, 2, 6), 6)

    test.expect_equal(math.clamp(Vector2.new(1, 2), {2, 3}, {6, 7}), Vector2.new(2, 3))
    test.expect_equal(math.clamp(Vector2.new(8, 9), {2, 3}, {6, 7}), Vector2.new(6, 7))

    test.expect_equal(math.clamp(Vector3.new(1, 2, 3), {2, 3, 4}, {6, 7, 8}), Vector3.new(2, 3, 4))
    test.expect_equal(math.clamp(Vector3.new(8, 9, 10), {2, 3, 4}, {6, 7, 8}), Vector3.new(6, 7, 8))

    test.expect_equal(math.clamp(Vector4.new(1, 2, 3, 4), {2, 3, 4, 5}, {6, 7, 8, 9}), Vector4.new(2, 3, 4, 5))
    test.expect_equal(math.clamp(Vector4.new(8, 9, 10, 11), {2, 3, 4, 5}, {6, 7, 8, 9}), Vector4.new(6, 7, 8, 9))
end)

test.test('fract', function()
    test.expect_num_equal(math.fract(1.2345), 0.2345, 0.0001)

    local r = math.fract(Vector2.new(1.2345, 1.2345))
    test.expect_num_equal(r:get(1), 0.2345, 0.0001)
    test.expect_num_equal(r:get(2), 0.2345, 0.0001)

    r = math.fract(Vector3.new(1.2345, 1.2345, 1.2345))
    test.expect_num_equal(r:get(1), 0.2345, 0.0001)
    test.expect_num_equal(r:get(2), 0.2345, 0.0001)
    test.expect_num_equal(r:get(3), 0.2345, 0.0001)

    r = math.fract(Vector4.new(1.2345, 1.2345, 1.2345, 1.2345))
    test.expect_num_equal(r:get(1), 0.2345, 0.0001)
    test.expect_num_equal(r:get(2), 0.2345, 0.0001)
    test.expect_num_equal(r:get(3), 0.2345, 0.0001)
    test.expect_num_equal(r:get(4), 0.2345, 0.0001)
end)