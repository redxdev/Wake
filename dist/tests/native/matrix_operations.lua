local test = require('test')

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

    test.expect_equal(5 + v2, Vector2.new(6, 7))
    test.expect_equal(5 + v3, Vector3.new(6, 7, 8))
    test.expect_equal(5 + v4, Vector4.new(6, 7, 8, 9))

    test.expect_equal(v2 + 5, Vector2.new(6, 7))
    test.expect_equal(v3 + 5, Vector3.new(6, 7, 8))
    test.expect_equal(v4 + 5, Vector4.new(6, 7, 8, 9))


    test.expect_equal(5 + m2x2, Matrix2x2.new(6, 7, 8, 9))

    test.expect_equal(m2x2 + 5, Matrix2x2.new(6, 7, 8, 9))
    test.expect_equal(m2x3 + 5, Matrix2x3.new(6, 7, 8, 9, 10, 11))
    test.expect_equal(m2x4 + 5, Matrix2x4.new(6, 7, 8, 9, 10, 11, 12, 13))


    test.expect_equal(5 + m3x3, Matrix3x3.new(6, 7, 8, 9, 10, 11, 12, 13, 14))

    test.expect_equal(m3x2 + 5, Matrix3x2.new(6, 7, 8, 9, 10, 11))
    test.expect_equal(m3x3 + 5, Matrix3x3.new(6, 7, 8, 9, 10, 11, 12, 13, 14))
    test.expect_equal(m3x4 + 5, Matrix3x4.new(6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17))

    test.expect_equal(5 + m4x4, Matrix4x4.new(6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21))

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

    --
    -- Errors
    --

    test.expect_error(function()
        local r = m4x2 + m4x4
    end)
end)

test.test('Subtraction', function()
    --
    -- Numbers
    --

    test.expect_equal(5 - v2, Vector2.new(4, 3))
    test.expect_equal(5 - v3, Vector3.new(4, 3, 2))
    test.expect_equal(5 - v4, Vector4.new(4, 3, 2, 1))

    test.expect_equal(v2 - 5, Vector2.new(-4, -3))
    test.expect_equal(v3 - 5, Vector3.new(-4, -3, -2))
    test.expect_equal(v4 - 5, Vector4.new(-4, -3, -2, -1))

    test.expect_equal(5 - m2x2, Matrix2x2.new(4, 3, 2, 1))

    test.expect_equal(m2x2 - 5, Matrix2x2.new(-4, -3, -2, -1))
    test.expect_equal(m2x3 - 5, Matrix2x3.new(-4, -3, -2, -1, 0, 1))
    test.expect_equal(m2x4 - 5, Matrix2x4.new(-4, -3, -2, -1, 0, 1, 2, 3))

    test.expect_equal(5 - m3x3, Matrix3x3.new(4, 3, 2, 1, 0, -1, -2, -3, -4))

    test.expect_equal(m3x2 - 5, Matrix3x2.new(-4, -3, -2, -1, 0, 1))
    test.expect_equal(m3x3 - 5, Matrix3x3.new(-4, -3, -2, -1, 0, 1, 2, 3, 4))
    test.expect_equal(m3x4 - 5, Matrix3x4.new(-4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7))

    test.expect_equal(5 - m4x4, Matrix4x4.new(4, 3, 2, 1, 0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11))

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

    --
    -- Errors
    --

    test.expect_error(function()
        local r = m3x3 - m2x4
    end)
end)

test.test('Multiplication', function()
    test.expect_equal(v2 * 2, Vector2.new(2, 4))
    test.expect_equal(v3 * 2, Vector3.new(2, 4, 6))
    test.expect_equal(v4 * 2, Vector4.new(2, 4, 6, 8))

    test.expect_equal(2 * v2, Vector2.new(2, 4))
    test.expect_equal(2 * v3, Vector3.new(2, 4, 6))
    test.expect_equal(2 * v4, Vector4.new(2, 4, 6, 8))

    test.expect_equal(m2x2 * 2, Matrix2x2.new(2, 4, 6, 8))
    test.expect_equal(m2x3 * 2, Matrix2x3.new(2, 4, 6, 8, 10, 12))
    test.expect_equal(m2x4 * 2, Matrix2x4.new(2, 4, 6, 8, 10, 12, 14, 16))

    test.expect_equal(2 * m2x2, Matrix2x2.new(2, 4, 6, 8))
    test.expect_equal(2 * m2x3, Matrix2x3.new(2, 4, 6, 8, 10, 12))
    test.expect_equal(2 * m2x4, Matrix2x4.new(2, 4, 6, 8, 10, 12, 14, 16))

    test.expect_equal(m3x2 * 2, Matrix3x2.new(2, 4, 6, 8, 10, 12))
    test.expect_equal(m3x3 * 2, Matrix3x3.new(2, 4, 6, 8, 10, 12, 14, 16, 18))
    test.expect_equal(m3x4 * 2, Matrix3x4.new(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24))

    test.expect_equal(2 * m3x2, Matrix3x2.new(2, 4, 6, 8, 10, 12))
    test.expect_equal(2 * m3x3, Matrix3x3.new(2, 4, 6, 8, 10, 12, 14, 16, 18))
    test.expect_equal(2 * m3x4, Matrix3x4.new(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24))

    test.expect_equal(m4x2 * 2, Matrix4x2.new(2, 4, 6, 8, 10, 12, 14, 16))
    test.expect_equal(m4x3 * 2, Matrix4x3.new(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24))
    test.expect_equal(m4x4 * 2, Matrix4x4.new(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32))

    test.expect_equal(2 * m4x2, Matrix4x2.new(2, 4, 6, 8, 10, 12, 14, 16))
    test.expect_equal(2 * m4x3, Matrix4x3.new(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24))
    test.expect_equal(2 * m4x4, Matrix4x4.new(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32))

    test.expect_equal(v2 * v2, Vector2.new(1, 4))
    test.expect_equal(v2 * m2x2, Vector2.new(5, 11))
    test.expect_equal(v2 * m3x2, Vector3.new(5, 11, 17))
    test.expect_equal(v2 * m4x2, Vector4.new(5, 11, 17, 23))

    test.expect_equal(v3 * v3, Vector3.new(1, 4, 9))
    test.expect_equal(v3 * m2x3, Vector2.new(14, 32))
    test.expect_equal(v3 * m3x3, Vector3.new(14, 32, 50))
    test.expect_equal(v3 * m4x3, Vector4.new(14, 32, 50, 68))

    test.expect_equal(v4 * v4, Vector4.new(1, 4, 9, 16))
    test.expect_equal(v4 * m2x4, Vector2.new(30, 70))
    test.expect_equal(v4 * m3x4, Vector3.new(30, 70, 110))
    test.expect_equal(v4 * m4x4, Vector4.new(30, 70, 110, 150))

    test.expect_equal(m2x2 * v2, Vector2.new(7, 10))
    test.expect_equal(m2x2 * m2x2, Matrix2x2.new(7, 10, 15, 22))
    test.expect_equal(m2x2 * m3x2, Matrix3x2.new(7, 10, 15, 22, 23, 34))
    test.expect_equal(m2x2 * m4x2, Matrix4x2.new(7, 10, 15, 22, 23, 34, 31, 46))

    test.expect_equal(m2x3 * v2, Vector3.new(9, 12, 15))
    test.expect_equal(m2x3 * m2x2, Matrix2x3.new(9, 12, 15, 19, 26, 33))
    test.expect_equal(m2x3 * m3x2, Matrix3x3.new(9, 12, 15, 19, 26, 33, 29, 40, 51))
    test.expect_equal(m2x3 * m4x2, Matrix4x3.new(9, 12, 15, 19, 26, 33, 29, 40, 51, 39, 54, 69))

    test.expect_equal(m2x4 * v2, Vector4.new(11, 14, 17, 20))
    test.expect_equal(m2x4 * m2x2, Matrix2x4.new(11, 14, 17, 20, 23, 30, 37, 44))
    test.expect_equal(m2x4 * m3x2, Matrix3x4.new(11, 14, 17, 20, 23, 30, 37, 44, 35, 46, 57, 68))
    test.expect_equal(m2x4 * m4x2, Matrix4x4.new(11, 14, 17, 20, 23, 30, 37, 44, 35, 46, 57, 68, 47, 62, 77, 92))

    test.expect_equal(m3x2 * v3, Vector2.new(22, 28))
    test.expect_equal(m3x2 * m2x3, Matrix2x2.new(22, 28, 49, 64))
    test.expect_equal(m3x2 * m3x3, Matrix3x2.new(22, 28, 49, 64, 76, 100))
    test.expect_equal(m3x2 * m4x3, Matrix4x2.new(22, 28, 49, 64, 76, 100, 103, 136))

    test.expect_equal(m3x3 * v3, Vector3.new(30, 36, 42))
    test.expect_equal(m3x3 * m2x3, Matrix2x3.new(30, 36, 42, 66, 81, 96))
    test.expect_equal(m3x3 * m3x3, Matrix3x3.new(30, 36, 42, 66, 81, 96, 102, 126, 150))
    test.expect_equal(m3x3 * m4x3, Matrix4x3.new(30, 36, 42, 66, 81, 96, 102, 126, 150, 138, 171, 204))

    test.expect_equal(m3x4 * v3, Vector4.new(38, 44, 50, 56))
    test.expect_equal(m3x4 * m2x3, Matrix2x4.new(38, 44, 50, 56, 83, 98, 113, 128))
    test.expect_equal(m3x4 * m3x3, Matrix3x4.new(38, 44, 50, 56, 83, 98, 113, 128, 128, 152, 176, 200))
    test.expect_equal(m3x4 * m4x3, Matrix4x4.new(38, 44, 50, 56, 83, 98, 113, 128, 128, 152, 176, 200, 173, 206, 239, 272))

    test.expect_equal(m4x2 * v4, Vector2.new(50, 60))
    test.expect_equal(m4x2 * m2x4, Matrix2x2.new(50, 60, 114, 140))
    test.expect_equal(m4x2 * m3x4, Matrix3x2.new(50, 60, 114, 140, 178, 220))
    test.expect_equal(m4x2 * m4x4, Matrix4x2.new(50, 60, 114, 140, 178, 220, 242, 300))

    test.expect_equal(m4x3 * v4, Vector3.new(70, 80, 90))
    test.expect_equal(m4x3 * m2x4, Matrix2x3.new(70, 80, 90, 158, 184, 210))
    test.expect_equal(m4x3 * m3x4, Matrix3x3.new(70, 80, 90, 158, 184, 210, 246, 288, 330))
    test.expect_equal(m4x3 * m4x4, Matrix4x3.new(70, 80, 90, 158, 184, 210, 246, 288, 330, 334, 392, 450))

    test.expect_equal(m4x4 * v4, Vector4.new(90, 100, 110, 120))
    test.expect_equal(m4x4 * m2x4, Matrix2x4.new(90, 100, 110, 120, 202, 228, 254, 280))
    test.expect_equal(m4x4 * m3x4, Matrix3x4.new(90, 100, 110, 120, 202, 228, 254, 280, 314, 356, 398, 440))
    test.expect_equal(m4x4 * m4x4, Matrix4x4.new(90, 100, 110, 120, 202, 228, 254, 280, 314, 356, 398, 440, 426, 484, 542, 600))

    test.expect_error(function()
        local r = m2x3 * m4x4
    end)
end)