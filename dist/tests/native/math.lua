local test = require('test')
local math = math

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