cmake_minimum_required(VERSION 3.15)
project(cpp_life)

set(CMAKE_CXX_STANDARD 20)



set(SFML_DIR "${PROJECT_SOURCE_DIR}/SFML/lib/cmake/SFML")
IF (CMAKE_BUILD_TYPE MATCHES Debug)
    set(SFML_BIN "${PROJECT_SOURCE_DIR}/SFML/bin-debug")
    set(BIN_DIR "${PROJECT_SOURCE_DIR}/build/debug")
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    set(SFML_BIN "${PROJECT_SOURCE_DIR}/SFML/bin-release")
    set(BIN_DIR "${PROJECT_SOURCE_DIR}/build/release")
ENDIF()



set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR})


find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
find_package(OpenGL)

add_executable(cpp_life main.cpp Grid.h Cell.h Grid.cpp Cell.cpp Cursor.h Cursor.cpp
        WorldMember.h WorldMember.cpp ThreadPool.h CellRule.h OverflowArray.h
        RLE_Coder.h Controls.h Controls.cpp CellRule.cpp
        imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp imgui/imgui-SFML.cpp imgui/imgui_demo.cpp imgui/imfilebrowser.h hlife.h Point.h Timer.h AppLog.h)
add_custom_command(
        TARGET cpp_life
        POST_BUILD

        COMMENT "copying from ${SFML_BIN}/ to ${BIN_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SFML_BIN} ${BIN_DIR}
)

include_directories(SFML/include)
include_directories(imgui)
include_directories(imgui-sfml)
target_link_libraries(cpp_life sfml-graphics sfml-audio ${OPENGL_gl_LIBRARY})
