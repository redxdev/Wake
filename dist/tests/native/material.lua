local test = require('test')
local Material = Material
local assets = assets

test.suite('Material Library')

test.test('Import demo material', function()
    require('materials.demo_lighting')
end)

test.test('clone', function()
    local mat1 = require('materials.demo_lighting')
    local mat2 = mat1:clone()

    mat2:setFloat('lightAmbience', 999)
    mat2:setFloat('foo', 123)
    test.expect_equal(mat1:getParameterCount(), mat2:getParameterCount() - 1)
    test.expect_not_equal(mat1:getParameter('lightAmbience'), mat2:getParameter('lightAmbience'))
    test.expect_equal(mat2:getParameter('foo'), 123)
    test.expect_equal(mat1:getParameter('foo'), nil)
    test.expect_equal(mat1:getTypeName(), 'materials.demo_lighting')
    test.expect_equal(mat2:getTypeName(), 'materials.demo_lighting')
    test.expect_not_equal(mat1:getShader(), nil)
    test.expect_not_equal(mat2:getShader(), nil)

    mat2:setTexture('tex1', assets.loadTexture('assets/textures/default.png'))
    test.expect_not_equal(mat2:getTexture('tex1'), nil)
    test.expect_equal(mat2:getTextureCount(), 1)
    test.expect_equal(mat1:getTexture('tex1'), nil)
    test.expect_equal(mat1:getTextureCount(), 0)
end)

test.test('copyFrom', function()
    local base = require('materials.demo_lighting')
    local mat = Material.new()
    mat:setFloat('lightAmbience', 876)
    mat:setVec4('foo', {1, 2, 3, 4})

    mat:copyFrom(base)
    test.expect_equal(base:getParameterCount(), mat:getParameterCount() - 1)
    test.expect_equal(mat:getParameter('lightAmbience'), 876)
    test.expect_num_equal(mat:getParameter('minBrightness'), 0.15, 0.000001)
end)

test.test('global', function()
    test.expect_not_equal(Material.getGlobal(), nil)
end)