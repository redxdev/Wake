print("Startup at " .. engine.getTime())

local shader = Shader.new(
[[
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec3 outNormal;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    outNormal = normal;
}
]],
[[
#version 330 core
in vec3 outNormal;

out vec4 outColor;

void main()
{
    vec3 lightColor = vec3(0.8, 0.8, 0.8);
    vec3 lightDirection = vec3(0.707, 0.707, 0);
    float lightAmbience = 0.8;

    float diffuseIntensity = max(0.0, dot(normalize(outNormal), -lightDirection));
    outColor = vec4(lightColor, 1.0) * vec4(lightColor * (lightAmbience * diffuseIntensity), 1.0);
}
]]
)

local shaderView = shader:getUniform("view")
local shaderProj = shader:getUniform("projection")
local shaderModel = shader:getUniform("model")

local obj = assets.loadOBJ("assets/sponza.obj")
if obj == nil then
    print("Uh oh!")
    engine.stop()
    return
end

print("Model has " .. #obj .. " meshes")

engine.setClearColor(1, 1, 1, 1)

local view = math.lookAt({2.5, 0.5, 0}, {0, 0.5, 0}, {0, 1, 0})
local projection = math.perspective(math.radians(45), 800 / 600, 0.1, 1000)

engine.tick:bind(function(dt)
    shader:use()
    shaderView:setMatrix4(view)
    shaderProj:setMatrix4(projection)

    local rot = engine.getTime() / 2
    local mat = math.scale{0.002, 0.002, 0.002}--math.rotate(rot, {1, 0, 0})
    --mat = math.translate(mat, {math.sin(engine.getTime() / 2) * 2, 0, 0})
    --mat = math.scale(mat, {0.002, 0.002, 0.002})
    shaderModel:setMatrix4(mat)

    for _,v in ipairs(obj) do
        v:draw()
    end
end)

engine.quit:bind(function()
    print("Shutdown at " .. engine.getTime())
end)
