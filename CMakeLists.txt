cmake_minimum_required(VERSION 3.15)
project(cpp_life)

set(CMAKE_CXX_STANDARD 20)
set(SFML_DIR "${PROJECT_SOURCE_DIR}/SFML/lib/cmake/SFML")
set(SFML_BIN_D "${PROJECT_SOURCE_DIR}/SFML/bin-debug")
set(SFML_BIN_R "${PROJECT_SOURCE_DIR}/SFML/bin-release")
set(BIN_DIR "${PROJECT_SOURCE_DIR}/build")
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(cpp_life main.cpp Grid.h Cell.h Grid.cpp Cell.cpp Cursor.h Cursor.cpp WorldMember.h WorldMember.cpp ThreadPool.h CellRule.h OverflowArray.h RLE_Coder.h)
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
target_link_libraries(cpp_life sfml-graphics sfml-audio)
