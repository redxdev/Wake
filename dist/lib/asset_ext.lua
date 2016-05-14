local assets = assets

function assets.loadMaterials(model)
    local materials = model:getMaterials()
    for k,matInfo in ipairs(materials) do
        pcall(function()
            local baseMat = require(matInfo.material:getTypeName())
            matInfo.material:copyFrom(baseMat)
        end)
    end
end