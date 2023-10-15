cmake_minimum_required(VERSION 3.26)
project(game C)

set(CMAKE_C_STANDARD 11)

# Add Allegro header and library directories
include_directories(/opt/homebrew/Cellar/allegro/5.2.8.0_2/include)
link_directories(/opt/homebrew/Cellar/allegro/5.2.8.0_2/lib)

add_executable(game main.c ./lib/dsa/linkedList/linkedList.c)

target_link_libraries(game allegro allegro_font allegro_primitives allegro_main)
