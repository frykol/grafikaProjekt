cmake_minimum_required(VERSION 3.20)

project(grafikaProjekt)

find_package(OpenGL REQUIRED)
find_package(GLEW)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glm)


add_executable(grafikaProjekt ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp 
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/Shader.cpp 
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/Application.cpp 
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/Mesh.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/stb_image.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/Texture.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/MeshLoader.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_draw.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_tables.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_demo.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends/imgui_impl_glfw.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp
                                )

target_include_directories(grafikaProjekt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends)
target_include_directories(grafikaProjekt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include)
target_include_directories(grafikaProjekt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/glm)

target_link_libraries(grafikaProjekt PUBLIC OpenGL::GL GLEW::GLEW glm glfw)

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources
            $<TARGET_FILE_DIR:grafikaProjekt>/resources
)