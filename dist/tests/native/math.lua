local test = require('test')
local math = math

test.suite('Math Extension Library')

test.test('perspective', function()
    local pers = math.perspective(45, 4/3, 0, 1000)
    local expected = {1.344443, 0, 0, 0, 0, 1.792591, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0 }
    local actual = pers:table()
    for i=1,#expected do
        test.expect_num_equal(expected[i], actual[i], 0.00001)
    end
end)