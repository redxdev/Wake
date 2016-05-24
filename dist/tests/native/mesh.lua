local test = require('test')
local Vertex = Vertex
local Mesh = Mesh
local tostring = tostring

test.suite('Mesh Library')

test.test('creation', function()
    local m = Mesh.new()
    test.assert_equal(#m:getVertices(), 0)
    test.assert_equal(#m:getIndices(), 0)

    m = Mesh.new{Vertex.new{1, 2, 3}, Vertex.new{4, 5, 6}}
    test.assert_equal(#m:getVertices(), 2)
    test.assert_equal(#m:getIndices(), 2)
    test.expect_equal(m:getVertices()[1], Vertex.new{1, 2, 3})
    test.expect_equal(m:getVertices()[2], Vertex.new{4, 5, 6})
    test.expect_equal(m:getIndices()[1], 0)
    test.expect_equal(m:getIndices()[2], 1)

    m = Mesh.new({Vertex.new{1, 2, 3}, Vertex.new{4, 5, 6}}, {1, 2, 3})
    test.assert_equal(#m:getVertices(), 2)
    test.assert_equal(#m:getIndices(), 3)
    test.expect_equal(m:getVertices()[1], Vertex.new{1, 2, 3})
    test.expect_equal(m:getVertices()[2], Vertex.new{4, 5, 6})
    test.expect_equal(m:getIndices()[1], 1)
    test.expect_equal(m:getIndices()[2], 2)
    test.expect_equal(m:getIndices()[3], 3)

    m = Mesh.new(m)
    test.assert_equal(#m:getVertices(), 2)
    test.assert_equal(#m:getIndices(), 3)
    test.expect_equal(m:getVertices()[1], Vertex.new{1, 2, 3})
    test.expect_equal(m:getVertices()[2], Vertex.new{4, 5, 6})
    test.expect_equal(m:getIndices()[1], 1)
    test.expect_equal(m:getIndices()[2], 2)
    test.expect_equal(m:getIndices()[3], 3)

    test.expect_error(Mesh.new, 1, 2, 3, 4, 5)
    test.expect_error(Vertex.new, 1, 2, 3, 4, 5)

end)

test.test('setters', function()
    local m = Mesh.new()
    m:setVertices{Vertex.new{1, 2, 3}, Vertex.new{4, 5, 6}}
    test.assert_equal(#m:getVertices(), 2)
    test.assert_equal(#m:getIndices(), 0)
    test.expect_equal(m:getVertices()[1], Vertex.new{1, 2, 3})
    test.expect_equal(m:getVertices()[2], Vertex.new{4, 5, 6})


    m:setVertices({Vertex.new{1, 2, 3}, Vertex.new{4, 5, 6}}, false)
    test.assert_equal(#m:getVertices(), 2)
    test.assert_equal(#m:getIndices(), 0)
    test.expect_equal(m:getVertices()[1], Vertex.new{1, 2, 3})
    test.expect_equal(m:getVertices()[2], Vertex.new{4, 5, 6})

    m:setVertices({Vertex.new{1, 2, 3}, Vertex.new{4, 5, 6}}, true)
    test.assert_equal(#m:getVertices(), 2)
    test.assert_equal(#m:getIndices(), 2)
    test.expect_equal(m:getVertices()[1], Vertex.new{1, 2, 3})
    test.expect_equal(m:getVertices()[2], Vertex.new{4, 5, 6})
    test.expect_equal(m:getIndices()[1], 0)
    test.expect_equal(m:getIndices()[2], 1)

    m:setIndices{1, 2, 3}
    test.assert_equal(#m:getVertices(), 2)
    test.assert_equal(#m:getIndices(), 3)
    test.expect_equal(m:getVertices()[1], Vertex.new{1, 2, 3})
    test.expect_equal(m:getVertices()[2], Vertex.new{4, 5, 6})
    test.expect_equal(m:getIndices()[1], 1)
    test.expect_equal(m:getIndices()[2], 2)
    test.expect_equal(m:getIndices()[3], 3)
end)

test.test('tostring', function()
    local m = Mesh.new({Vertex.new{1, 2, 3}, Vertex.new{4, 5, 6}}, {1, 2, 3})
    test.expect_equal(tostring(m), "Mesh[2,3]")

    local v = Vertex.new{1, 2, 3 }
    test.expect_equal(tostring(v), "Vertex({1,2,3},{1,0,0},{0,0})")
end)