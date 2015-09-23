local test = require('test')
require('stringutil')

test.suite('String Utility Library')

test.test('explode', function()
    local res = string.explode('|', "a|b|c|d|e|f|ghi|jkl")
    test.assert_equal(#res, 8)
    for k,v in ipairs({'a','b','c','d','e','f','ghi','jkl'}) do
        test.assert_equal(res[k], v)
    end
end)

test.test('trim', function()
    test.assert_equal(('  \t\thello\nworld\n\n'):trim(), 'hello\nworld')
    test.assert_equal(('hello world'):trim(), 'hello world')
end)