local Camera = require('camera')
local config = require('config/cfg')
local material = require('materials/demo_lighting'):clone()

local shader = material:getShader()

local shaderView = shader:getUniform("view")
local shaderProj = shader:getUniform("projection")
local shaderModel = shader:getUniform("model")
local shaderTex = shader:getUniform("tex1")

local obj = assets.loadModel("assets/models/cube.wmdl")
if obj == nil then
    print("Unable to load model.")
    return
end

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

    material:activate()

    local mat = math.scale{1, 1, 1}
    shaderModel:setMatrix4(mat)

    shaderView:setMatrix4(cam:getViewMatrix())
    shaderProj:setMatrix4(cam.projection)

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