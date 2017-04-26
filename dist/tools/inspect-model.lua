function hook_engine_tool()
    local args = wake.getArguments()
    if #args ~= 1 and #args ~= 2 then
        print("Usage: inspect-model <input_model>")
        print("Description: Display information about a model.")
        return false
    end

    local inputPath = args[1]

    print("Loading input from " .. inputPath)
    local input = assets.loadModel(inputPath)
    if input == nil then
        print("Unable to load input model.")
        return false
    end

    local metadata = input:getMetadata()
    print("Metadata:")
    print("\tSource: " .. metadata.source)
    print("\tVersion: " .. metadata.version)
    print("\tPath: " .. metadata.path)

    print("Materials: " .. input:getMaterialCount())

    local materials = input:getMaterials()
    for _, m in ipairs(materials) do
        if m ~= nil then
            print("\t" .. m.name .. " - " .. m.material:getTypeName() .. " - " .. m.material:getTextureCount() .. " textures, " .. m.material:getParameterCount() .. " parameters")
            for tn, t in pairs(m.material:getTextures()) do
                print("\t\t" .. tn .. " - " .. t:getPath())
            end
        end
    end

    print("Meshes: " .. input:getMeshCount())

    local indices = 0
    local vertices = 0
    local meshes = input:getMeshes()
    for _, c in ipairs(meshes) do
        local mesh = c.mesh
        if mesh ~= nil then
            indices = indices + #mesh:getIndices()
            vertices = vertices + #mesh:getVertices()
        end
    end

    print("Indices: " .. indices)
    print("Triangles: " .. (indices / 3))
    print("Vertices: " .. vertices)

    return true
end
