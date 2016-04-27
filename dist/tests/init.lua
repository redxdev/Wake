--
-- Require any and all tests you need here, in the order you want them to run.
--
require('tests/lib/class')
require('tests/lib/stringutil')

require('tests/native/vector2')
require('tests/native/vector3')
require('tests/native/vector4')

require('tests/native/matrix2x2')
require('tests/native/matrix2x3')
require('tests/native/matrix2x4')
require('tests/native/matrix3x2')
require('tests/native/matrix3x3')
require('tests/native/matrix3x4')
require('tests/native/matrix4x2')
require('tests/native/matrix4x3')
require('tests/native/matrix4x4')

require('tests/native/matrix_operations')

require('tests/native/vertex')
require('tests/native/mesh')

--
-- Do not touch the code below this line.
--
local test = require('test')

function hook_engine_tests()
    return test.run_all()
end