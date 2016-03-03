print("Startup at " .. engine.getTime())

local shader = Shader.new(
[[
#version 330 core
layout (location = 0) in vec2 position;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
}
]],
[[
#version 330 core
uniform vec3 color;

out vec4 outColor;

void main()
{
    outColor = vec4(color, 1.0);
}
]]
)

local color = shader:getUniform("color")

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
    color:set3f((math.sin(time)+1)/2, (math.cos(time)+1)/2, ((math.sin(time)+1)/2 + (math.cos(time)+1)/2)/2)
end)

engine.quit:bind(function()
    print("Shutdown at " .. engine.getTime())
end)
