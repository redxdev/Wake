local assets = assets

function assets.loadMaterials(model)
    local materials = model:getMaterials()
    for _, matInfo in ipairs(materials) do
        if matInfo ~= nil and matInfo.material ~= nil then
            local status, err = pcall(function()
                local baseMat = require(matInfo.material:getTypeName())
                matInfo.material:copyFrom(baseMat)
            end)

            if not status then
                print("assets.loadMaterials warning: unable to resolve material " .. matInfo.material:getTypeName())
                print("  - error: " .. tostring(err))
            end
        end
    end
end