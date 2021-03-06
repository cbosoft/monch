cmake_minimum_required(VERSION 3.19)
project(monch)

set(CMAKE_CXX_STANDARD 14)

## Shaders
file(READ "shaders/vertex/vertex.glsl" VERTEX_SHADER_SOURCE_MULTILINE)
string(REPLACE "\n" "\\n\\\n" VERTEX_SHADER_SOURCE "${VERTEX_SHADER_SOURCE_MULTILINE}")
file(READ "shaders/fragment/fragment.glsl" FRAGMENT_SHADER_SOURCE_MULTILINE)
string(REPLACE "\n" "\\n\\\n" FRAGMENT_SHADER_SOURCE "${FRAGMENT_SHADER_SOURCE_MULTILINE}")
file(READ "shaders/fragment/font.glsl" FONT_SHADER_SOURCE_MULTILINE)
string(REPLACE "\n" "\\n\\\n" FONT_SHADER_SOURCE "${FONT_SHADER_SOURCE_MULTILINE}")
file(READ "shaders/fragment/texture.glsl" TEXTURE_SHADER_SOURCE_MULTILINE)
string(REPLACE "\n" "\\n\\\n" TEXTURE_SHADER_SOURCE "${TEXTURE_SHADER_SOURCE_MULTILINE}")
configure_file("${CMAKE_SOURCE_DIR}/src/monch/rendering/renderer/shaders.h.in" "${CMAKE_SOURCE_DIR}/src/monch/rendering/renderer/shaders.h")

########################################################################################################################
## Libs

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
list(APPEND EXTRA_LIBS ${FREETYPE_LIBRARIES})
set(GLFW_LIB glfw)

find_package(nlohmann_json REQUIRED)
list(APPEND EXTRA_LIBS ${nlohmann_json_LIBRARIES})

if (${CMAKE_SYSTEM_NAME} STREQUAL Darwin)
    message("System is MacOS.")
    find_package(GLFW3 REQUIRED)
    include_directories(${GLFW3_INCLUDE_DIRS})
    add_compile_definitions(MACOS)
    add_compile_definitions(GL_SILENCE_DEPRECATION)
    list(APPEND EXT_LIB "-framework OpenGL")
    set(FONT_PATH "$ENV{HOME}/Library/Fonts")
    set(FONT_FALLBACK_PATH "/System/Library/Fonts")
    set(TITLEBAR_SOURCE "src/monch/rendering/renderer/title_bar/macos.mm")
    set_property(SOURCE src/monch/rendering/renderer/title_bar/macos.mm PROPERTY COMPILE_FLAGS "-x objective-c++")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
    message("System is Linux.")
    list(APPEND EXTRA_LIBS X11)
    add_compile_definitions(LINUX)
    set(TITLEBAR_SOURCE "src/monch/rendering/renderer/title_bar/x11.cpp")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL WINDOWS)
    message("System is Windows.")
    set(GLFW_LIB glfw3)
    add_compile_definitions(WINDOWS)
    set(TITLEBAR_SOURCE "src/monch/rendering/renderer/title_bar/windows.cpp")
else()
    message("UNKNOWN SYSTEM!")
endif()

## Font path
configure_file("${CMAKE_SOURCE_DIR}/src/monch/rendering/font/manager/font_path.h.in" "${CMAKE_SOURCE_DIR}/src/monch/rendering/font/manager/font_path.h")

########################################################################################################################
## Main
add_executable(${PROJECT_NAME}
        src/monch/main.cpp
        src/monch/object/Object.cpp
        src/monch/rendering/renderer/Renderer.cpp
        src/monch/editor/app/EditorApp.cpp
        src/monch/rendering/renderable/Renderable.cpp
        src/monch/rendering/renderable/Renderable.h
        src/monch/util.h
        src/monch/rendering/renderer/shaders.cpp
        src/monch/rendering/gl.h
        src/monch/rendering/quad/Quad.cpp
        src/monch/rendering/quad/Quad.h
        src/monch/rendering/renderer/error.cpp
        src/monch/rendering/font/font/ctor.cpp
        src/monch/rendering/font/font/dtor.cpp
        src/monch/rendering/font/font/get.cpp
        src/monch/rendering/font/font/load.cpp
        src/monch/rendering/font/manager/get_font.cpp
        src/monch/rendering/font/manager/ctor.cpp
        src/monch/rendering/font/manager/dtor.cpp
        src/monch/rendering/font/manager/singleton.cpp
        src/monch/rendering/renderer/input_callbacks.cpp
        src/monch/object/position.cpp
        src/monch/editor/text_area/TextArea.cpp
        src/monch/editor/text_area/TextArea.h
        src/monch/editor/rendered_character/RenderedCharacter.cpp
        src/monch/editor/rendered_character/RenderedCharacter.h
        src/monch/rendering/container/Container.cpp
        src/monch/rendering/container/Container.h
        ${TITLEBAR_SOURCE}
        src/monch/object/invalidate.cpp
        src/monch/events/manager/EventManager.cpp
        src/monch/events/manager/EventManager.h
        src/monch/events/event/Event.h
        src/monch/object/find.cpp
        src/monch/object/size.cpp
        src/monch/rendering/Vertex.h
        src/monch/editor/app/layout.cpp
        src/monch/rendering/quad/renderable/RenderableQuad.cpp
        src/monch/rendering/quad/renderable/RenderableQuad.h
        src/monch/editor/scrollable_area/ScrollableArea.cpp src/monch/editor/scrollable_area/ScrollableArea.h)

target_link_libraries(${PROJECT_NAME} PUBLIC ${GLFW_LIB} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${EXTRA_LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/src" ${OPENGL_INCLUDE_DIR} ${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIR})