local test = require('test')
local Material = Material

test.suite('Material Library')

test.test('Import demo material', function()
    require('materials.demo_lighting')
end)

test.test('clone', function()
    local mat1 = require('materials.demo_lighting')
    local mat2 = mat1:clone()

    mat2:setFloat('lightAmbience', 999)
    mat2:setFloat('foo', 123)
    test.expect_not_equal(mat1:getParameter('lightAmbience'), mat2:getParameter('lightAmbience'))
    test.expect_equal(mat2:getParameter('foo'), 123)
    test.expect_equal(mat1:getParameter('foo'), nil)
end)