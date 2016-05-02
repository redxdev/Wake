local test = require('test')
local assets = assets

test.suite('Assets Library')

test.test('loadModel', function()
    local model = assets.loadModel('assets/teapot.obj')
    test.assert_not_equal(model, nil)

    test.expect_equal(#model, 2)
    for _,mesh in ipairs(model) do
        test.expect_equal(type(mesh), 'userdata')
        test.expect(#mesh:getVertices() > 0)
        test.expect(#mesh:getIndices() > 0)
    end
end)

test.test('loadWModel', function()
    local model = assets.loadWModel('assets/teapot.wmdl')
    test.assert_not_equal(model, nil)

    test.expect_equal(#model, 2)
    for _,mesh in ipairs(model) do
        test.expect_equal(type(mesh), 'userdata')
        test.expect(#mesh:getVertices() > 0)
        test.expect(#mesh:getIndices() > 0)
    end
end)