local test = require('test')
local Vertex = Vertex
local Vector3 = Vector3
local Vector2 = Vector2

test.suite('Vertex Library')

test.test('creation', function()
    local v = Vertex.new()
    test.assert_equal(v:getPosition(), Vector3.new(0, 0, 0))
    test.assert_equal(v:getNormal(), Vector3.new(1, 0, 0))
    test.assert_equal(v:getTexCoords(), Vector2.new(0, 0))

    v = Vertex.new{1, 2, 3}
    test.assert_equal(v:getPosition(), Vector3.new(1, 2, 3))
    test.assert_equal(v:getNormal(), Vector3.new(1, 0, 0))
    test.assert_equal(v:getTexCoords(), Vector2.new(0, 0))

    v = Vertex.new({1, 2, 3}, {4, 5, 6})
    test.assert_equal(v:getPosition(), Vector3.new(1, 2, 3))
    test.assert_equal(v:getNormal(), Vector3.new(4, 5, 6))
    test.assert_equal(v:getTexCoords(), Vector2.new(0, 0))

    v = Vertex.new({1, 2, 3}, {4, 5, 6}, {7, 8})
    test.assert_equal(v:getPosition(), Vector3.new(1, 2, 3))
    test.assert_equal(v:getNormal(), Vector3.new(4, 5, 6))
    test.assert_equal(v:getTexCoords(), Vector2.new(7, 8))
end)

test.test('setters', function()
    local v = Vertex.new()
    v:setPosition{1, 2, 3}
    v:setNormal{4, 5, 6}
    v:setTexCoords{7, 8 }
    test.assert_equal(v:getPosition(), Vector3.new(1, 2, 3))
    test.assert_equal(v:getNormal(), Vector3.new(4, 5, 6))
    test.assert_equal(v:getTexCoords(), Vector2.new(7, 8))
end)