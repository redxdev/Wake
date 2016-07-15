local running = true

function exit()
    running = false
    print("Goodbye!")
end

function help()
    print("Wake Engine " .. wake.getVersion() .. " console")
    print("Use 'exit' to exit, or 'help' to print help text.")
    print()
    print("Enter a valid line of lua (or one of the above commands) to run it.")
    print("If the line returns a value other than nil, the result will be printed.")
    print("Local variables will not work, as the line is executed in a function")
    print("that is discarded after use.")
end

function hook_engine_tool()
    print("Wake Engine " .. wake.getVersion() .. " console")
    print("Use 'exit' to exit, or 'help' to print help text.")

    while running do
        io.write(">> ")
        local line = io.read()
        if line == "exit" then
            exit()
        elseif line == "help" then
            help()
        else
            local f, err = loadstring(line)
            if f == nil then
                print(err)
            else
                local status, err = pcall(function()
                    local result = f()
                    if result ~= nil then
                        print(result)
                    end
                end)

                if not status then
                    print(err)
                end
            end
        end
    end

    return true
end