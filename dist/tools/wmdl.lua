function hook_engine_tool()
    local args = wake.getArguments()
    if #args ~= 2 and #args ~= 3 then
        print("Usage: wmdl <input_model> <output_model> [compress=true]")
        print("Description: Converts models into the wake model format. The wake model format")
        print("             is faster for the engine to load than most formats, and is")
        print("             compressed in order to save space.")
        return false
    end

    local inputPath = args[1]
    local outputPath = args[2]
    local compress = true
    if #args == 3 then
        compress = args[3] == "true"
    end

    print("Loading input from " .. inputPath)
    local input = assets.loadModel(inputPath)
    if input == nil then
        print("Unable to load input model.")
        return false
    end

    if compress then
        print("Saving output with compression to " .. outputPath)
    else
        print("Saving output to " .. outputPath)
    end
    local result = assets.saveModel(outputPath, input, compress)
    if not result then
        print("Unable to save output model.")
        return false
    end

    print("Saved output to " .. outputPath)
    return true
end
