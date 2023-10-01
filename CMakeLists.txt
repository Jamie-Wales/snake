cmake_minimum_required(VERSION 3.26)
project(game C)

set(CMAKE_C_STANDARD 99)

# Add Allegro header and library directories
include_directories(/opt/homebrew/Cellar/allegro/5.2.8.0_2/include)
link_directories(/opt/homebrew/Cellar/allegro/5.2.8.0_2/lib)
set(LINKED_LIST_SOURCE lib/dsa/linkedList/linkedList.c)

add_library(linkedList ${LINKED_LIST_SOURCE})

add_executable(game main.c)

target_link_libraries(game allegro allegro_font allegro_primitives allegro_main linkedList)
