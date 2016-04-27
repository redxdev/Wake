print("Startup at " .. engine.getTime())

local shader = Shader.new(
[[
#version 330 core
layout (location = 0) in vec3 position;

void main()
{
    gl_Position = vec4(position, 1.0);
}
]],
[[
#version 330 core
uniform float time;

out vec4 outColor;

void main()
{
    outColor = vec4((sin(time) + 1) / 2, (cos(time) + 1) / 2, ((sin(time) + 1) / 2 + (cos(time) + 1) / 2) / 2, 1.0);
}
]]
)

local shaderTime = shader:getUniform("time")

local mesh = Mesh.new({
    Vertex.new{-0.5, -0.5, 0},
    Vertex.new{0.5, -0.5, 0},
    Vertex.new{-0.5, 0.5, 0},
    Vertex.new{0.5, 0.5, 0}},
    {0, 1, 2, 2, 1, 3}
)

engine.setClearColor(1, 1, 1, 1)

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

    shader:use()
    shaderTime:set1f(engine.getTime())

    mesh:draw()
end)

engine.quit:bind(function()
    print("Shutdown at " .. engine.getTime())
end)
