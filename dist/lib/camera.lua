local Vector3 = Vector3
local Quat = Quat
local math = math
local class = require('class')

local Camera = class()

function Camera:construct(pos, orientation, up, projection)
    self.position = pos or Vector3.new(0, 0, 0)
    self.orientation = Vector3.new(0, 0, 0)
    self.up = up or Vector3.new(0, 1, 0)
    self.projection = projection or math.perspective(math.radians(45), 4/3, 0.1, 1000)
end

function Camera:getViewMatrix()
    return math.lookAt(self.position, self.position + self:getFrontVector(), self.up)
end

function Camera:getFrontVector()
    return Vector3.new(1, 0, 0) * Quat.new(math.radians(self.orientation))
end

function Camera:addRotation(amount)
    self.orientation = self.orientation + amount
    self.orientation:set(3, math.clamp(self.orientation:get(3), -89, 89))
end

function Camera:moveForward(amount)
    self.position = self.position + amount * self:getFrontVector()
end

function Camera:moveRight(amount)
    self.position = self.position + amount * self:getFrontVector():cross(self.up)
end

return Camera