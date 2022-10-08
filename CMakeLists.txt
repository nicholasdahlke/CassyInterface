cmake_minimum_required(VERSION 3.23)
project(CassyInterface)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(hidapi)
find_package(glfw3 REQUIRED)
find_package(imgui REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(imgui)
include_directories(${OPENGL_INCLUDE_DIRS})
set(sources
        imgui/imconfig.h
        imgui/imgui.cpp
        imgui/imgui.h
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_internal.h
        imgui/imgui_widgets.cpp
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h
        imgui/imgui_tables.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_glfw.h
        imgui/backends/imgui_impl_opengl3.cpp
        imgui/backends/imgui_impl_opengl3.h
        implot/implot.h
        implot/implot_internal.h
        implot/implot.cpp
        implot/implot_items.cpp
        )


add_executable(CassyInterface main.cpp Cassy.h Cassy.cpp SerialPort.cpp SerialPort.h Gui.cpp Gui.h ${sources})

target_link_libraries(CassyInterface PRIVATE hidapi::hidapi glfw ${OPENGL_LIBRARIES})