print("Startup at " .. engine.getTime())

local shader = Shader.new(
[[
#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(position, 1.0);
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
local shaderView = shader:getUniform("view")
local shaderProj = shader:getUniform("projection")

local mesh = Mesh.new({
    Vertex.new{-1, -1, 1},
    Vertex.new{1, -1, 1},
    Vertex.new{1, 1, 1},
    Vertex.new{-1, 1, 1},
    Vertex.new{-1, -1, -1},
    Vertex.new{1, -1, -1},
    Vertex.new{1, 1, -1},
    Vertex.new{-1, 1, -1}
    },
    {
        2, 1, 0,
        0, 3, 2,

        6, 5, 1,
        1, 2, 6,

        5, 6, 7,
        7, 4, 5,

        3, 0, 4,
        4, 7, 3,

        1, 5, 4,
        4, 0, 1,

        6, 2, 3,
        3, 7, 6
    }
)

engine.setClearColor(1, 1, 1, 1)

local view = math.lookAt({4, 3, 3}, {0, 0, 0}, {0, 1, 0})
local projection = math.perspective(45, 800 / 600, 0.1, 1000)

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
    shaderView:setMatrix4(view)
    shaderProj:setMatrix4(projection)

    mesh:draw()
end)

engine.quit:bind(function()
    print("Shutdown at " .. engine.getTime())
end)
