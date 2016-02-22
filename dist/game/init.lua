print("Startup at " .. engine.getTime())

local shader = Shader.new(
[[
#version 150
in vec2 position;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
}
]],
[[
#version 150
uniform vec3 triangleColor;

out vec4 outColor;

void main()
{
    outColor = vec4(triangleColor, 1.0);
}
]]
)

local triangleColor = shader:getUniform("triangleColor")
triangleColor:set3f(1, 0, 0)

local time = 10
local next = time - 1
engine.tick:bind(function(dt)
    time = time - dt
    if time < next then
        print(next)
        next = next - 1
    end

    if time < 0 then
        engine.stop()
    end
end)

engine.quit:bind(function()
    print("Shutdown at " .. engine.getTime())
end)