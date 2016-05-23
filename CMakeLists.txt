cmake_minimum_required(VERSION 3.2)
project(Wake)

option(COVERALLS "Turn on coveralls support" OFF)
option(COVERALLS_UPLOAD "Upload the generated coveralls json" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
set(WAKE_DISTRIBUTION_PATH "${CMAKE_CURRENT_SOURCE_DIR}/dist")

include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
string(SUBSTRING ${GIT_SHA1} 0 7 WAKE_REVISION)
configure_file("src/build.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/build.wake.cpp" @ONLY)

find_package(OpenGL REQUIRED)
find_package(Lua REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(assimp REQUIRED)
find_package(Snappy REQUIRED)

if (WIN32 AND NOT CYGWIN)
    find_package(GLFW REQUIRED)
else ()
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    set(GLFW_LIBRARIES ${GLFW_STATIC_LIBRARIES})
endif ()

if (MSVC)
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif ()
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif ()

if (MINGW)
    if (CMAKE_BUILD_TYPE MATCHES Release)
        # Due to a bug in MinGW, we need to define this in release mode.
        add_definitions(-D__NO_INLINE__)
    endif ()
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(TCLAP_INCLUDE_DIR "deps/tclap-1.2.1/include")
set(STB_INCLUDE_DIR "deps/stb")

set(INCLUDE_DIRECTORIES
        "include"
        ${TCLAP_INCLUDE_DIR}
        ${OBJLOADER_INCLUDE_DIR}
        ${LUA_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIR}
        ${assimp_INCLUDE_DIRS}
        ${SNAPPY_INCLUDE_DIR}
        ${STB_INCLUDE_DIR}
        )

set(SOURCE_FILES
        "${CMAKE_CURRENT_BINARY_DIR}/build.wake.cpp"

        "src/engine.cpp"
        "src/glutil.cpp"
        "src/input.cpp"
        "src/luautil.cpp"
        "src/main.cpp"
        "src/material.cpp"
        "src/mesh.cpp"
        "src/model.cpp"
        "src/moduleregistry.cpp"
        "src/pushvalue.cpp"
        "src/scriptmanager.cpp"
        "src/shader.cpp"
        "src/texture.cpp"
        "src/wake.cpp"
        "src/wmdl.cpp"

        "src/bindings/luaassets.cpp"
        "src/bindings/luaengine.cpp"
        "src/bindings/luaevent.cpp"
        "src/bindings/luainput.cpp"
        "src/bindings/luamaterial.cpp"
        "src/bindings/luamath.cpp"
        "src/bindings/luamatrix.cpp"
        "src/bindings/luamesh.cpp"
        "src/bindings/luamodel.cpp"
        "src/bindings/luaquat.cpp"
        "src/bindings/luashader.cpp"
        "src/bindings/luatexture.cpp"
        "src/bindings/luawake.cpp"
        )

set(EXT_SOURCE_FILES
        "deps/stb/stb_image.c"
        )

if (COVERALLS)
    # Coverage only supported on GCC at this time.
    if (CMAKE_COMPILER_IS_GNUCXX)
        include(Coveralls)
        coveralls_turn_on_coverage()
        coveralls_setup(
                "${SOURCE_FILES}"
                ${COVERALLS_UPLOAD}
                "${CMAKE_CURRENT_SOURCE_DIR}/CMake"
        )
    endif ()
endif ()

add_definitions(-DGLM_FORCE_RADIANS)

include_directories(${INCLUDE_DIRECTORIES})
add_executable(Wake ${SOURCE_FILES} ${EXT_SOURCE_FILES})
target_link_libraries(Wake
        ${LUA_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${GLFW_LIBRARIES}
        ${OPENGL_gl_LIBRARY}
        ${OPENGL_glu_LIBRARY}
        ${assimp_LIBRARIES}
        ${SNAPPY_LIBRARIES}
        )

enable_testing()
add_test(NAME LuaTests
        COMMAND Wake --testing
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/dist")