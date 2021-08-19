configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../include/General/Version.hpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/../include/General/Version.hpp
        @ONLY)

set(NC_GENERATED
        ${CMAKE_CURRENT_BINARY_DIR}/../include/General/Version.hpp)

set(NC_INCLUDES
        ../include/Components/VelocityComponent.hpp
        ../include/Components/PlayerComponent.hpp
        ../include/Components/PlayerInputComponent.hpp
        ../include/Components/InventoryComponent.hpp
        ../include/Game/Game.hpp
        ../include/Game/GameState.hpp
        ../include/Game/MainMenuState.hpp
        ../include/Game/PlayingState.hpp
        ../include/Game/GameRegistry.hpp
        ../include/Game/Item.hpp
        ../include/Game/ItemStack.hpp
        ../include/General/TextureAtlas.hpp
        ../include/General/Object.hpp
        ../include/General/InputHandler.hpp
        ../include/General/Physics.hpp
        ../include/UI/UI.hpp
        ../include/UI/Widget.hpp
        ../include/UI/ImageWidget.hpp
        ../include/UI/MainMenu.hpp
        ../include/UI/ButtonWidget.hpp
        ../include/UI/PlayerUI.hpp
        ../include/World/Map.hpp
        ../include/World/Tile.hpp
        ../include/World/Chunk.hpp
        ../include/World/Generator.hpp
        ../include/World/OverworldGenerator.hpp)

set(NC_SOURCES
        ${imgui_sfml_src}
        Components/InventoryComponent.cpp
        Game/Game.cpp
        Game/GameState.cpp
        Game/MainMenuState.cpp
        Game/PlayingState.cpp
        Game/GameRegistry.cpp
        Game/Item.cpp
        Game/ItemStack.cpp
        General/main.cpp
        General/TextureAtlas.cpp
        General/Object.cpp
        General/InputHandler.cpp
        General/Physics.cpp
        UI/UI.cpp
        UI/Widget.cpp
        UI/ImageWidget.cpp
        UI/MainMenu.cpp
        UI/ButtonWidget.cpp
        UI/PlayerUI.cpp
        World/Map.cpp
        World/Tile.cpp
        World/Chunk.cpp
        World/Generator.cpp
        World/OverworldGenerator.cpp)

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