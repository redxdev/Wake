function hook_engine_tool()
    local args = wake.getArguments()
    if #args ~= 1 and #args ~= 2 then
        print("Usage: inspect-model <input_model> [mesh_index]")
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
	
	print("Meshes = " .. #input)
	local vertices = 0
	local indices = 0
	for _,mesh in ipairs(input) do
		vertices = vertices + #mesh:getVertices()
		indices = indices + #mesh:getIndices()
	end
	
	print("Vertices = " .. vertices)
	print("Indices = " .. indices)
	print("Triangles = " .. (indices/3))
	
	if #args == 2 then
		local meshIndex = tonumber(args[2])
		if meshIndex == nil or meshIndex <= 0 or meshIndex > #input then
			print("Invalid mesh index " .. args[2])
			return false
		end
		
		local mesh = input[meshIndex]
		print("Mesh #" .. meshIndex .. ":")
		print("\tVertices = " .. #mesh:getVertices())
		print("\tIndices = " .. #mesh:getIndices())
		print("\tTriangles = " .. (#mesh:getIndices()/3))
	end
	
    return true
end
