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

	print("Components: " .. input:getComponentCount())

    local indices = 0
    local vertices = 0
    local components = input:getComponents()
    for _,c in ipairs(components) do
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
