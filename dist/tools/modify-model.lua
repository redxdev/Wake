local function cmd_add_material(model, matName)
    local mat = Material.new()

    if model:addMaterial(matName, mat) then
        print("Added material " .. matName)
    else
        print("Unable to add material " .. matName .. ", does a material with that name already exist?")
        return false
    end
end

local function cmd_set_material_type(model, matName, typeName)
    local mat = model:getMaterialByName(matName)
    if mat == nil then
        print("Unknown material name " .. matName)
        return false
    end

    mat.material:setTypeName(typeName)
    print("Set material " .. matName .. " to type " .. typeName)
    return true
end

function hook_engine_tool()
    local args = wake.getArguments()
    if #args < 2 then
        print("Usage: modify-model <input_model> <command> [args...]")
        print("Description: Modify a model on disk. The model must be in WMDL format (use the wmdl tool to create a")
        print("             model in this format). All save operations are done with default settings. If you want")
        print("             to change those options, use the wmdl tool to export the resulting model to itself.")
        print("Commands:")
        print("             add-material <name>")
        print("             set-material-type <name> <type_name>")
        return false
    end

    local input_model = args[1]
    local command = args[2]

    local model = assets.loadModel(input_model)
    if model == nil then
        print("Unable to load model.")
        return false
    end

    local metadata = model:getMetadata()
    if metadata.source ~= "wmdl" then
        print("Model must be a wmdl. Use the wmdl tool to create a wmdl from this file.")
        return false
    end

    if command == "add-material" then
        if #args ~= 3 then
            print("modify-model <input_model> add-material <name>")
            return false
        end
        if not cmd_add_material(model, args[3]) then
            return false
        end
    elseif command == "set-material-type" then
        if #args ~= 4 then
            print("modify-model <input_model> set-material-type <name> <type_name>")
            return false
        end
        if not cmd_set_material_type(model, args[3], args[4]) then
            return false
        end
    else
        print("Unknown command " .. command)
        return false
    end

    print("Saving...")
    local result = assets.saveModel(input_model, model)
    if result then
        print("Saved!")
        return true
    else
        print("Unable to save model.")
        return false
    end
end