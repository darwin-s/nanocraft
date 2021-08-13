configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../include/Version.hpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/../include/Version.hpp
        @ONLY)

set(NC_GENERATED
        ${CMAKE_CURRENT_BINARY_DIR}/../include/Version.hpp)

set(NC_INCLUDES
        ../include/Game.hpp
        ../include/TextureAtlas.hpp
        ../include/Map.hpp
        ../include/Tile.hpp
        ../include/Chunk.hpp
        ../include/Object.hpp
        ../include/VelocityComponent.hpp
        ../include/PlayerComponent.hpp
        ../include/PlayerInputComponent.hpp
        ../include/InputHandler.hpp
        ../include/Physics.hpp
        ../include/Generator.hpp
        ../include/OverworldGenerator.hpp)

set(NC_SOURCES
        ${imgui_sfml_src}
        main.cpp
        Game.cpp
        TextureAtlas.cpp
        Map.cpp
        Tile.cpp
        Chunk.cpp
        Object.cpp
        InputHandler.cpp
        Physics.cpp
        Generator.cpp
        OverworldGenerator.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS system network window audio graphics REQUIRED)
add_executable(nanocraft WIN32 ${NC_SOURCES} ${NC_INCLUDES} ${NC_GENERATED})
target_link_libraries(nanocraft PUBLIC
        fastnoiselite
        spdlog
        imgui-sfml
        json
        physfs-static
        entt sfml-graphics
        sfml-audio
        sfml-network
        sfml-window
        sfml-system)

if (WIN32)
    target_link_libraries(nanocraft PUBLIC sfml-main)
endif (WIN32)

target_compile_features(nanocraft PUBLIC cxx_std_17)
set_target_properties(nanocraft PROPERTIES
        FOLDER "Binaries"
        CXX_EXTENSIONS OFF
        INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../binaries
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../binaries
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../binaries
        PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../binaries)

target_include_directories(nanocraft PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../include>
        $<INSTALL_INTERFACE:include>
        PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_definitions(nanocraft PRIVATE "$<$<CONFIG:DEBUG>:NC_DEBUG>")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/../include" PREFIX "Header Files" FILES ${NC_INCLUDES})

install(TARGETS nanocraft
        RUNTIME DESTINATION bin)