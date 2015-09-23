cmake_minimum_required(VERSION 3.2)
project(Wake)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
set(WAKE_DISTRIBUTION_PATH "${CMAKE_CURRENT_SOURCE_DIR}/dist")

include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
string(SUBSTRING ${GIT_SHA1} 0 8 WAKE_REVISION)
configure_file("src/build.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/build.cpp" @ONLY)

find_package(OpenGL REQUIRED)
find_package(Lua REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLM REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(TCLAP_INCLUDE_DIR "deps/tclap-1.2.1/include")

set(INCLUDE_DIRECTORIES
        "include"
        ${TCLAP_INCLUDE_DIR}
        ${LUA_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIR}
        ${GLM_INCLUDE_DIR}
)

set(SOURCE_FILES
        "${CMAKE_CURRENT_BINARY_DIR}/build.cpp"
        "src/main.cpp"
        "src/wake.cpp"
        "src/pushvalue.cpp"
        "src/moduleregistry.cpp"
        "src/scriptmanager.cpp"
        "src/bindings/luaevent.cpp"
        "src/bindings/luamatrix.cpp"
)

add_definitions(-DGLM_FORCE_RADIANS)

include_directories(${INCLUDE_DIRECTORIES})
add_executable(Wake ${SOURCE_FILES})
target_link_libraries(Wake
        ${LUA_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${GLFW_LIBRARIES}
)