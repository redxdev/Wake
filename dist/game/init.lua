local Camera = require('camera')
local config = require('config/cfg')

local shader = Shader.new(
[[
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec3 outNormal;
out vec2 outTexCoords;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    outNormal = normal;
    outTexCoords = texCoords;
}
]],
[[
#version 330 core
in vec3 outNormal;
in vec2 outTexCoords;

out vec4 outColor;

uniform sampler2D tex1;

void main()
{
    vec4 texColor = texture(tex1, outTexCoords);
    vec3 lightColor = vec3(0.8, 0.8, 0.8);
    vec3 lightDirection = normalize(vec3(1.0, -1.0, 0.6));
    float lightAmbience = 0.8;

    float diffuseIntensity = max(0.3, dot(normalize(outNormal), -lightDirection));
    outColor = vec4(lightColor, 1.0) * vec4(lightColor * (lightAmbience * diffuseIntensity) * texColor.rgb, 1.0);
}
]]
)

local shaderView = shader:getUniform("view")
local shaderProj = shader:getUniform("projection")
local shaderModel = shader:getUniform("model")
local shaderTex = shader:getUniform("tex1")

local obj = assets.loadModel("assets/models/cube.wmdl")
if obj == nil then
    print("Unable to load model.")
    return
end

local tex = assets.loadTexture("assets/textures/default.png")

engine.setClearColor(1, 1, 1, 1)

local cam = Camera.new(Vector3.new(-2.5, 0, 0))
local speed = 1
local fastSpeed= 2

engine.tick:bind(function(dt)
    local moveSpeed = speed
    if input.getKey(input.key.LeftShift) == input.action.Press then
        moveSpeed = fastSpeed
    end

    if input.getKey(input.key.W) == input.action.Press then
        cam:moveForward(moveSpeed * dt)
    end

    if input.getKey(input.key.S) == input.action.Press then
        cam:moveForward(-moveSpeed * dt)
    end

    if input.getKey(input.key.A) == input.action.Press then
        cam:moveRight(-moveSpeed * dt)
    end

    if input.getKey(input.key.D) == input.action.Press then
        cam:moveRight(moveSpeed * dt)
    end

    shader:use()
    shaderView:setMatrix4(cam:getViewMatrix())
    shaderProj:setMatrix4(cam.projection)

    local mat = math.scale{1, 1, 1}
    shaderModel:setMatrix4(mat)

    tex:activate(0)
    shaderTex:set1i(0)

    for _,v in ipairs(obj) do
        v:draw()
    end
end)

engine.quit:bind(function()
    print("Shutdown at " .. engine.getTime())
end)

input.event.key:bind(function(key, action)
    if key == input.key.Escape and action == input.action.Release then
        engine.stop()
    end
end)

local lastX = 0
local lastY = 0
local firstMouse = true
input.setCursorMode(input.cursorMode.Disabled)
input.event.cursorPos:bind(function(x, y)
    if firstMouse then
        firstMouse = false
        lastX = x
        lastY = y
    end

    local xOffset = (x - lastX) * config.input.mouseSensitivity
    local yOffset = (y - lastY) * config.input.mouseSensitivity

    cam:addRotation(Vector3.new{0, xOffset, yOffset})

    lastX = x
    lastY = y
end)