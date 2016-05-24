--
-- Wake test suite: A very simple unit testing library.
--

local err = io.stdout -- keep these the same, otherwise we get issues in some console environments
local out = io.stdout
local insert = table.insert
local error = error
local ipairs = ipairs
local pcall = pcall
local assert = assert
local traceback = debug.traceback
local tostring = tostring
local abs = math.abs

require("stringutil")

local suites = {}
local current_suite
local current_test

local suite_filters = {}
local test_filters = {}

local module = {}

if wake.getEngineMode() ~= 'testing' then
	print("Warning: You are including the test library when in " .. wake.getEngineMode() .. " mode")
end

local function split_traceback(trace)
    return string.trim(string.explode('\n', trace)[3])
end

function module.suite(name)
    local suite = {
        name = name,
        success = {},
        fail = {},
        skip = {},
        tests = {}
    }
    insert(suites, suite)
    current_suite = suite
end

function module.test(name, f)
    if current_suite == nil then
        error("Cannot run a test when no suite is defined")
    end

    local test = {
        name = name,
        call = f,
        errors = {}
    }
    insert(current_suite.tests, test)
end

function module.filter_suites(pattern)
    insert(suite_filters, pattern)
end

function module.filter_tests(pattern)
    insert(test_filters, pattern)
end

function module.suites()
    return suites
end

function module.run_all()
    out:write('Engine ' .. wake.getVersion() .. ' running in ' .. wake.getEngineMode() .. ' mode.\n\n')

    if #suite_filters > 0 then
        out:write("Suite Filters:\n")
        for _,f in ipairs(suite_filters) do
            out:write("- " .. f .. "\n")
        end
    end

    if #test_filters > 0 then
        out:write("Test Filters:\n")
        for _,f in ipairs(test_filters) do
            out:write("- " .. f .. "\n")
        end
    end

    local fail = 0
    local success = 0
    local skipped = 0
    for _, v in ipairs(suites) do
        local r = module.run_suite(v)

        if r == "skipped" then
            skipped = skipped + 1
        elseif r then
            success = success + 1
        else
            fail = fail + 1
        end
        out:write('\n')
    end

    out:write('Results: ' .. success .. ' suites passed, ' .. fail .. ' suites failed, ' .. skipped .. ' suites skipped\n')

    if fail > 0 then
        return false
    else
        return true
    end
end

function module.run_suite(suite)
    local shouldRun = false
    if #suite_filters == 0 then
        shouldRun = true
    else
        for _,pattern in ipairs(suite_filters) do
            if string.find(suite.name, pattern) then
                shouldRun = true
                break
            end
        end
    end

    if shouldRun then
        current_suite = suite

        out:write('== Suite "' .. suite.name .. '" ==\n')

        for _, v in ipairs(suite.tests) do
            local r = module.run_test(v)

            if r == "skipped" then
                insert(current_suite.skip, v)
            elseif r then
                insert(current_suite.success, v)
            else
                insert(current_suite.fail, v)
            end
        end

        if #(suite.fail) > 0 then
            err:write('!! Suite "' .. suite.name .. '" failed with ' .. #(suite.fail) .. ' errors !!\n')
            return false
        else
            if #suite.skip > 0 then
                err:write('== Suite "' .. suite.name .. '" succeeded (' .. #(suite.skip) .. ' tests skipped) ==\n')
            else
                err:write('== Suite "' .. suite.name .. '" succeeded ==\n')
            end
            return true
        end
    else
        out:write('== Suite "' .. suite.name .. '" == SKIPPED\n')
        return "skipped"
    end
end

function module.run_test(test)
    local shouldRun = false
    if #test_filters == 0 then
        shouldRun = true
    else
        for _,pattern in ipairs(test_filters) do
            if string.find(test.name, pattern) then
                shouldRun = true
                break
            end
        end
    end

    if shouldRun then
        current_test = test

        out:write('  Test "' .. test.name .. '"... ')

        local status, e = pcall(test.call)
        if status and #(test.errors) == 0 then
            out:write('passed\n')
            return true
        else
            out:write('failed\n')

            if not status then
                err:write('-- ' .. e .. '\n')
            end

            for _, v in ipairs(test.errors) do
                err:write('-- ' .. v .. '\n')
            end

            return false
        end
    else
        out:write('  Test "' .. test.name .. '"... SKIPPED\n')
        return "skipped"
    end
end

function module.assert(check, msg)
    if not check then
        if msg then
            error("Assertion failed at " .. split_traceback(traceback()) .. ": " .. msg)
        else
            error("Assertion failed at " .. split_traceback(traceback()))
        end
    end
end

function module.expect(check, msg)
    if not check then
        if msg then
            insert(current_test.errors, "Expectation failed at " .. split_traceback(traceback()) .. ": " .. msg)
        else
            insert(current_test.errors, "Expectation failed at " .. split_traceback(traceback()))
        end
    end
end

function module.assert_equal(a, b, msg)
    if a ~= b then
        if msg then
            error("Assertion failed at " .. split_traceback(traceback()) .. " (" .. tostring(a) .. " != " .. tostring(b) .. "): " .. msg)
        else
            error("Assertion failed at " .. split_traceback(traceback()) .. " (" .. tostring(a) .. " != " .. tostring(b) .. ")")
        end
    end
end

function module.expect_equal(a, b, msg)
    if a ~= b then
        if msg then
            insert(current_test.errors, "Expectation failed at " .. split_traceback(traceback()) .. " (" .. tostring(a) .. " != " .. tostring(b) .. "): " .. msg)
        else
            insert(current_test.errors, "Expectation failed at " .. split_traceback(traceback()) .. " (" .. tostring(a) .. " != " .. tostring(b) .. ")")
        end
    end
end

function module.assert_not_equal(a, b, msg)
    if a == b then
        if msg then
            error("Assertion failed at " .. split_traceback(traceback()) .. " (" .. tostring(a) .. " == " .. tostring(b) .. "): " .. msg)
        else
            error("Assertion failed at " .. split_traceback(traceback()) .. " (" .. tostring(a) .. " == " .. tostring(b) .. ")")
        end
    end
end

function module.expect_not_equal(a, b, msg)
    if a == b then
        if msg then
            insert(current_test.errors, "Expectation failed at " .. split_traceback(traceback()) .. " (" .. tostring(a) .. " == " .. tostring(b) .. "): " .. msg)
        else
            insert(current_test.errors, "Expectation failed at " .. split_traceback(traceback()) .. " (" .. tostring(a) .. " == " .. tostring(b) .. ")")
        end
    end
end

function module.assert_num_equal(a, b, precision, msg)
    precision = precision or 2.22044604925031308e-16
    if abs(a - b) > precision then
        if msg then
            error("Assertion failed at " .. split_traceback(traceback()) .. " (" .. tostring(a) .. " != " .. tostring(b) .. " at precision " .. tostring(precision) .. "): " .. msg)
        else
            error("Assertion failed at " .. split_traceback(traceback()) .. " (" .. tostring(a) .. " != " .. tostring(b) .. " at precision " .. tostring(precision) .. ")")
        end
    end
end

function module.assert_num_not_equal(a, b, precision, msg)
    precision = precision or 2.22044604925031308e-16
    if abs(a - b) <= precision then
        if msg then
            error("Assertion failed at " .. split_traceback(traceback()) .. " (" .. tostring(a) .. " == " .. tostring(b) .. " at precision " .. tostring(precision) .. "): " .. msg)
        else
            error("Assertion failed at " .. split_traceback(traceback()) .. " (" .. tostring(a) .. " == " .. tostring(b) .. " at precision " .. tostring(precision) .. ")")
        end
    end
end

function module.expect_num_equal(a, b, precision, msg)
    precision = precision or 2.22044604925031308e-16
    if abs(a - b) > precision then
        if msg then
            insert(current_test.errors, "Assertion failed at " .. split_traceback(traceback()) .. " (" .. tostring(a) .. " != " .. tostring(b) .. " at precision " .. tostring(precision) .. "): " .. msg)
        else
            insert(current_test.errors, "Assertion failed at " .. split_traceback(traceback()) .. " (" .. tostring(a) .. " != " .. tostring(b) .. " at precision " .. tostring(precision) .. ")")
        end
    end
end

function module.expect_num_not_equal(a, b, precision, msg)
    precision = precision or 2.22044604925031308e-16
    if abs(a - b) <= precision then
        if msg then
            insert(current_test.errors, "Assertion failed at " .. split_traceback(traceback()) .. " (" .. tostring(a) .. " == " .. tostring(b) .. " at precision " .. tostring(precision) .. "): " .. msg)
        else
            insert(current_test.errors, "Assertion failed at " .. split_traceback(traceback()) .. " (" .. tostring(a) .. " == " .. tostring(b) .. " at precision " .. tostring(precision) .. ")")
        end
    end
end

--
-- Do not use further assert/expect function inside of the "error" class of assertions/expectations
--

function module.assert_error(func, ...)
    if pcall(func, ...) then
        error("Assertion failed at " .. split_traceback(traceback()) .. " (error expected)")
    end
end

function module.expect_error(func, ...)
    if pcall(func, ...) then
        insert(current_test.errors, "Assertion failed at " .. split_traceback(traceback()) .. " (error expected)")
    end
end

function module.assert_no_error(func, ...)
    if not pcall(func, ...) then
        error("Assertion failed at " .. split_traceback(traceback()) .. " (no error expected)")
    end
end

function module.expect_no_error(func, ...)
    if not pcall(func, ...) then
        insert(current_test.errors, "Assertion failed at " .. split_traceback(traceback()) .. " (no error expected)")
    end
end

return module