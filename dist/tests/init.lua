--
-- Require any and all tests you need here, in the order you want them to run.
--
require('tests.lib.class')
require('tests.lib.stringutil')
require('tests.lib.event')

require('tests.native.vector2')
require('tests.native.vector3')
require('tests.native.vector4')

require('tests.native.matrix2x2')
require('tests.native.matrix2x3')
require('tests.native.matrix2x4')
require('tests.native.matrix3x2')
require('tests.native.matrix3x3')
require('tests.native.matrix3x4')
require('tests.native.matrix4x2')
require('tests.native.matrix4x3')
require('tests.native.matrix4x4')

require('tests.native.matrix_operations')

require('tests.native.quat')

require('tests.native.math')

require('tests.native.vertex')
require('tests.native.mesh')

require('tests.native.assets')
require('tests.native.material')

--
-- Do not touch the code below this line.
--
local test = require('test')

function hook_engine_tests()
    local args = wake.getArguments()
    if #args > 0 then
        local i = 1
        while i <= #args do
            local arg = args[i]
            if arg == "--list-tests" or arg == "-l" then
                for _,suite in ipairs(test.suites()) do
                    print(suite.name)
                    for _,t in ipairs(suite.tests) do
                        print("- " .. t.name)
                    end
                end
                return true
            else
                print("Usage: Wake -t -- [options]")
                print("Options:")
                print("--list-tests    -t    List all tests and suites.")
                return false
            end
        end
    end

    return test.run_all()
end