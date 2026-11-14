cmake_minimum_required(VERSION 3.20)
project(Light_Simulation LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Light_Simulation
    src/main.cpp
    src/app/App.cpp
    src/camera/FlyCamera.cpp
    src/core/Assets.cpp
    src/core/Color.cpp
    src/geometry/Box.cpp
    src/geometry/Mesh.cpp
    src/geometry/Model.cpp
    src/lighting/Lights.cpp
    src/render/GLDebug.cpp
    src/render/IndexBuffer.cpp
    src/render/Material.cpp
    src/render/Shader.cpp
    src/render/Texture.cpp
    src/render/VertexArray.cpp
    src/render/VertexBuffer.cpp
    src/scene/Scene.cpp
    src/window/GLFWindow.cpp
    # Third-party code compiled into the program
    externals/src/glad.c
    externals/src/stb_image.cpp
)

target_include_directories(Light_Simulation PRIVATE include)

# Lets the program find assets/ even when the build directory is outside the
# source tree (see src/core/Assets.cpp).
target_compile_definitions(Light_Simulation PRIVATE
    LIGHTSIM_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

find_package(OpenGL REQUIRED)

if(MSVC)
    # Windows: use the prebuilt GLFW and Assimp shipped in externals/.
    target_include_directories(Light_Simulation SYSTEM PRIVATE externals/includes)
    target_link_libraries(Light_Simulation PRIVATE
        OpenGL::GL
        ${CMAKE_CURRENT_SOURCE_DIR}/externals/libs/glfw3.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/externals/libs/assimp-vc145-mtd.lib)

    # The Assimp DLL has to sit next to the executable.
    add_custom_command(TARGET Light_Simulation POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/externals/libs/assimp-vc145-mtd.dll
            $<TARGET_FILE_DIR:Light_Simulation>)

    target_compile_options(Light_Simulation PRIVATE /W4 /permissive- /external:W0)
else()
    # Linux/macOS: use system packages, e.g.
    #   Debian/Ubuntu: sudo apt install libglfw3-dev libassimp-dev
    #   macOS:         brew install glfw assimp
    find_package(glfw3 3.3 REQUIRED)
    find_package(assimp REQUIRED)
    target_link_libraries(Light_Simulation PRIVATE OpenGL::GL glfw assimp::assimp ${CMAKE_DL_LIBS})

    # externals/includes also holds Windows builds of the GLFW and Assimp
    # headers, which must not shadow the system ones. Expose only the
    # header-only libraries (glad, KHR, glm, stb) from a copy in the build tree.
    set(THIRD_PARTY_INCLUDE ${CMAKE_CURRENT_BINARY_DIR}/third_party_include)
    file(COPY
        ${CMAKE_CURRENT_SOURCE_DIR}/externals/includes/glad
        ${CMAKE_CURRENT_SOURCE_DIR}/externals/includes/KHR
        ${CMAKE_CURRENT_SOURCE_DIR}/externals/includes/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/externals/includes/stb
        DESTINATION ${THIRD_PARTY_INCLUDE})
    target_include_directories(Light_Simulation SYSTEM PRIVATE ${THIRD_PARTY_INCLUDE})

    target_compile_options(Light_Simulation PRIVATE -Wall -Wextra)
endif()
