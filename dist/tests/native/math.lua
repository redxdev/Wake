local test = require('test')
local math = math

test.suite('Math Extension Library')

test.test('perspective', function()
    local pers = math.perspective(45, 4/3, 0, 1000)
    local expected = {1.344443, 0, 0, 0, 0, 1.792591, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0 }
    local actual = pers:table()
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