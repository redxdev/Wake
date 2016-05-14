local assets = assets

function assets.loadMaterials(model)
    local materials = model:getMaterials()
    for k, matInfo in ipairs(materials) do
        if matInfo ~= nil and matInfo.material ~= nil then
            if not pcall(function()
                local baseMat = require(matInfo.material:getTypeName())
                matInfo.material:copyFrom(baseMat)
            end) then
                print("assets.loadMaterials warning: unable to resolve material " .. matInfo.material:getTypeName())
            end
        end
    end
end