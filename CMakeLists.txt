cmake_minimum_required(VERSION 3.26)
project(Genius_Invokation_TCG_2 C)

set(CMAKE_C_STANDARD 99)

include_directories(include)
add_subdirectory(src)

target_link_libraries(Genius_Invokation_TCG_2 PRIVATE
        mingw32
        SDL2main
        SDL2
        SDL2_image
        SDL2_ttf
        SDL2_mixer
)
