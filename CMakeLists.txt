cmake_minimum_required(VERSION 3.15)
project(nanocraft 
    LANGUAGES CXX
    VERSION 0.1.0.0)

add_subdirectory(3rdparty)
add_subdirectory(src)

install(TARGETS nanocraft DESTINATION nanocraft)
install(DIRECTORY res/data DESTINATION nanocraft)