add_subdirectory(entt)
add_subdirectory(json)
add_subdirectory(physfs)
target_include_directories(physfs-static PUBLIC physfs/include)
add_library(imgui-sfml INTERFACE)
target_include_directories(imgui-sfml INTERFACE imgui/include)
set(imgui_sfml_src
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui-SFML.cpp
    PARENT_SCOPE)
add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE spdlog/include)
add_library(fastnoiselite INTERFACE)
target_include_directories(fastnoiselite INTERFACE fastnoiselite)