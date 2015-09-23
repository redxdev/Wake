local test = require('test')
local type = type

local Vector2 = Vector2
local Vector3 = Vector3
local Vector4 = Vector4

local Matrix2x2 = Matrix2x2
local Matrix2x3 = Matrix2x3
local Matrix2x4 = Matrix2x4

local Matrix3x2 = Matrix3x2
local Matrix3x3 = Matrix3x3
local Matrix3x4 = Matrix3x4

local Matrix4x2 = Matrix4x2
local Matrix4x3 = Matrix4x3
local Matrix4x4 = Matrix4x4

test.suite('Matrix Operations')

local v2 = Vector2.new(1, 2)
local v3 = Vector3.new(1, 2, 3)
local v4 = Vector4.new(1, 2, 3, 4)

local m2x2 = Matrix2x2.new(1, 2, 3, 4)
local m2x3 = Matrix2x3.new(1, 2, 3, 4, 5, 6)
local m2x4 = Matrix2x4.new(1, 2, 3, 4, 5, 6, 7, 8)

local m3x2 = Matrix3x2.new(1, 2, 3, 4, 5, 6)
local m3x3 = Matrix3x3.new(1, 2, 3, 4, 5, 6, 7, 8, 9)
local m3x4 = Matrix3x4.new(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12)

local m4x2 = Matrix4x2.new(1, 2, 3, 4, 5, 6, 7, 8)
local m4x3 = Matrix4x3.new(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12)
local m4x4 = Matrix4x4.new(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)

test.test('Addition', function()
    --
    -- Numbers
    --

    test.expect_equal(v2 + 5, Vector2.new(6, 7))
    test.expect_equal(v3 + 5, Vector3.new(6, 7, 8))
    test.expect_equal(v4 + 5, Vector4.new(6, 7, 8, 9))

    test.expect_equal(m2x2 + 5, Matrix2x2.new(6, 7, 8, 9))
    test.expect_equal(m2x3 + 5, Matrix2x3.new(6, 7, 8, 9, 10, 11))
    test.expect_equal(m2x4 + 5, Matrix2x4.new(6, 7, 8, 9, 10, 11, 12, 13))

    test.expect_equal(m3x2 + 5, Matrix3x2.new(6, 7, 8, 9, 10, 11))
    test.expect_equal(m3x3 + 5, Matrix3x3.new(6, 7, 8, 9, 10, 11, 12, 13, 14))
    test.expect_equal(m3x4 + 5, Matrix3x4.new(6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17))

    test.expect_equal(m4x2 + 5, Matrix4x2.new(6, 7, 8, 9, 10, 11, 12, 13))
    test.expect_equal(m4x3 + 5, Matrix4x3.new(6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17))
    test.expect_equal(m4x4 + 5, Matrix4x4.new(6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21))

    --
    -- Matrices
    --

    test.expect_equal(v2 + v2, Vector2.new(2, 4))
    test.expect_equal(v3 + v3, Vector3.new(2, 4, 6))
    test.expect_equal(v4 + v4, Vector4.new(2, 4, 6, 8))

    test.expect_equal(m2x2 + m2x2, Matrix2x2.new(2, 4, 6, 8))
    test.expect_equal(m2x3 + m2x3, Matrix2x3.new(2, 4, 6, 8, 10, 12))
    test.expect_equal(m2x4 + m2x4, Matrix2x4.new(2, 4, 6, 8, 10, 12, 14, 16))

    test.expect_equal(m3x2 + m3x2, Matrix3x2.new(2, 4, 6, 8, 10, 12))
    test.expect_equal(m3x3 + m3x3, Matrix3x3.new(2, 4, 6, 8, 10, 12, 14, 16, 18))
    test.expect_equal(m3x4 + m3x4, Matrix3x4.new(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24))

    test.expect_equal(m4x2 + m4x2, Matrix4x2.new(2, 4, 6, 8, 10, 12, 14, 16))
    test.expect_equal(m4x3 + m4x3, Matrix4x3.new(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24))
    test.expect_equal(m4x4 + m4x4, Matrix4x4.new(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32))
end)

test.test('Subtraction', function()
    --
    -- Numbers
    --

    test.expect_equal(v2 - 5, Vector2.new(-4, -3))
    test.expect_equal(v3 - 5, Vector3.new(-4, -3, -2))
    test.expect_equal(v4 - 5, Vector4.new(-4, -3, -2, -1))

    test.expect_equal(m2x2 - 5, Matrix2x2.new(-4, -3, -2, -1))
    test.expect_equal(m2x3 - 5, Matrix2x3.new(-4, -3, -2, -1, 0, 1))
    test.expect_equal(m2x4 - 5, Matrix2x4.new(-4, -3, -2, -1, 0, 1, 2, 3))

    test.expect_equal(m3x2 - 5, Matrix3x2.new(-4, -3, -2, -1, 0, 1))
    test.expect_equal(m3x3 - 5, Matrix3x3.new(-4, -3, -2, -1, 0, 1, 2, 3, 4))
    test.expect_equal(m3x4 - 5, Matrix3x4.new(-4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7))

    test.expect_equal(m4x2 - 5, Matrix4x2.new(-4, -3, -2, -1, 0, 1, 2, 3))
    test.expect_equal(m4x3 - 5, Matrix4x3.new(-4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7))
    test.expect_equal(m4x4 - 5, Matrix4x4.new(-4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11))

    --
    -- Matrices
    --

    test.expect_equal(v2 - v2, Vector2.new(0, 0))
    test.expect_equal(v3 - v3, Vector3.new(0, 0, 0))
    test.expect_equal(v4 - v4, Vector4.new(0, 0, 0, 0))

    test.expect_equal(m2x2 - m2x2, Matrix2x2.new(0, 0, 0, 0))
    test.expect_equal(m2x3 - m2x3, Matrix2x3.new(0, 0, 0, 0, 0, 0))
    test.expect_equal(m2x4 - m2x4, Matrix2x4.new(0, 0, 0, 0, 0, 0, 0, 0))

    test.expect_equal(m3x2 - m3x2, Matrix3x2.new(0, 0, 0, 0, 0, 0))
    test.expect_equal(m3x3 - m3x3, Matrix3x3.new(0, 0, 0, 0, 0, 0, 0, 0, 0))
    test.expect_equal(m3x4 - m3x4, Matrix3x4.new(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0))

    test.expect_equal(m4x2 - m4x2, Matrix4x2.new(0, 0, 0, 0, 0, 0, 0, 0))
    test.expect_equal(m4x3 - m4x3, Matrix4x3.new(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0))
    test.expect_equal(m4x4 - m4x4, Matrix4x4.new(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0))
end)

test.test('Multiplication', function()

end)