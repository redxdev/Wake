local test = require('test')
local class = require('class')
local type = type

test.suite('Class Library')

test.test('class', function()
    local base = class()
    function base:foo() return 'bar' end

    local inst = base.new()
    test.assert(inst ~= nil)
    test.assert(type(inst) == 'table')

    test.assert(type(inst.construct) == 'function')

    test.assert(type(inst.class) == 'function')
    test.assert_equal(inst:class(), base)

    test.assert(type(inst.super) == 'function')
    test.assert_equal(inst:super(), nil)

    test.assert(type(inst.instanceOf) == 'function')
    test.assert(inst:instanceOf(base))
    test.assert(not inst:instanceOf(class()))

    test.assert(type(inst.foo) == 'function')
    test.assert_equal(inst:foo(), 'bar')
end)

test.test('class with superclass', function()
    local base = class()
    function base:foo() return 'bar' end
    function base:bar() return 'baz' end

    local child = class(base)
    function child:bar() return self:super():foo() end
    function child:baz() return 'foo' end

    local inst = child.new()
    test.assert(inst ~= nil)
    test.assert(type(inst) == 'table')

    test.assert(type(inst.construct) == 'function')

    test.assert(type(inst.class) == 'function')
    test.assert_equal(inst:class(), child)

    test.assert(type(inst.super) == 'function')
    test.assert_equal(inst:super(), base)

    test.assert(type(inst.instanceOf) == 'function')
    test.assert(inst:instanceOf(base))
    test.assert(inst:instanceOf(child))
    test.assert(not inst:instanceOf(class()))

    test.assert(type(inst.foo) == 'function')
    test.assert_equal(inst:foo(), 'bar')

    test.assert(type(inst.bar) == 'function')
    test.assert_equal(inst:bar(), 'bar')

    test.assert(type(inst.baz) == 'function')
    test.assert_equal(inst:baz(), 'foo')
end)

test.test('constructors', function()
    local base = class()
    function base:construct(val) self.foo = val end

    local inst = base.new('bar')
    test.assert(inst ~= nil)
    test.assert(type(inst) == 'table')

    test.assert_equal(inst.foo, 'bar')
end)