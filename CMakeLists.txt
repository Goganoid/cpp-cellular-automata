cmake_minimum_required(VERSION 3.15)
project(cpp_life)

set(CMAKE_CXX_STANDARD 20)
set(SFML_DIR "${PROJECT_SOURCE_DIR}/SFML/lib/cmake/SFML")
set(SFML_BIN_D "${PROJECT_SOURCE_DIR}/SFML/bin-debug")
set(SFML_BIN_R "${PROJECT_SOURCE_DIR}/SFML/bin-release")
set(BIN_DIR "${PROJECT_SOURCE_DIR}/build")



find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
find_package(OpenGL)

add_executable(cpp_life main.cpp Grid.h Cell.h Grid.cpp Cell.cpp Cursor.h Cursor.cpp
        WorldMember.h WorldMember.cpp ThreadPool.h CellRule.h OverflowArray.h
        RLE_Coder.h Controls.h Controls.cpp CellRule.cpp
        imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp imgui/imgui-SFML.cpp imgui/imgui_demo.cpp imgui/imfilebrowser.h hlife.h Point.h Timer.h)
add_custom_command(
        TARGET cpp_life
        POST_BUILD
        # DEBUG
        COMMENT "copying from ${SFML_BIN_D}/ to ${BIN_DIR}/debug"
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SFML_BIN_D} ${BIN_DIR}/debug
        # RELEASE
        COMMENT "copying from ${SFML_BIN_R}/ to ${BIN_DIR}/release"
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SFML_BIN_R} ${BIN_DIR}/release
)

include_directories(SFML/include)
include_directories(imgui)
include_directories(imgui-sfml)
target_link_libraries(cpp_life sfml-graphics sfml-audio ${OPENGL_gl_LIBRARY})
