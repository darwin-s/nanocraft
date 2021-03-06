// Copyright 2021 Sirbu Dan
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <Game/Game.hpp>
#include <Game/Item.hpp>
#include <General/Version.hpp>
#include <Game/MainMenuState.hpp>
#include <World/Chunk.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <physfs.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <algorithm>

namespace {

PHYSFS_EnumerateCallbackResult populateJson(const std::string& path,
                                            nlohmann::json& json) {
    PHYSFS_File* fileHandle = PHYSFS_openRead(path.c_str());
    PHYSFS_sint64 fileSize;
    PHYSFS_sint64 read;
    char* fileData = nullptr;

    if (fileHandle == NULL) {
        spdlog::warn("Could not open file {}!", path);
        return PHYSFS_ENUM_ERROR;
    } else {
        fileSize = PHYSFS_fileLength(fileHandle);
        if (fileSize == -1) {
            spdlog::warn("Could not retreive size for file {}!", path);
            return PHYSFS_ENUM_ERROR;
        }
        fileData = new char[fileSize];
        read     = PHYSFS_readBytes(fileHandle, fileData, fileSize);
        if (read < fileSize) {
            spdlog::warn("Could not read file {}!", path);
            delete[] fileData;
            return PHYSFS_ENUM_ERROR;
        }
        PHYSFS_close(fileHandle);
    }

    json = nlohmann::json::parse(fileData, fileData + fileSize);
    delete[] fileData;

    return PHYSFS_ENUM_OK;
}

PHYSFS_EnumerateCallbackResult
    loadTextureCallback(void* data, const char* origdir, const char* fname) {
    std::string texPath;
    texPath += origdir;
    texPath += '/';
    texPath += fname;

    if (!nc::Game::getInstance()->getTextureAtlas().addTexture(texPath)) {
        spdlog::error("Could not load texture {}", texPath);
    }

    return PHYSFS_ENUM_OK;
}

PHYSFS_EnumerateCallbackResult loadItemCallback(void* data, const char* origdir,
                                                const char* fname) {
    std::string path;
    path += origdir;
    path += '/';
    path += fname;

    nlohmann::json j;

    if (populateJson(path, j) != PHYSFS_ENUM_OK) {
        return PHYSFS_ENUM_OK;
    }

    nc::Item* i = new nc::Item;
    i->setName(j["name"].get<std::string>());
    i->setTexture(j["texture"].get<std::string>());
    if (j.contains("placeTile")) {
        i->setPlaceableTile(j["placeTile"].get<std::string>());
    }

    nc::Game::getInstance()->getRegistry().registerItem(i);

    return PHYSFS_ENUM_OK;
}

PHYSFS_EnumerateCallbackResult loadTileCallback(void* data, const char* origdir,
                                                const char* fname) {
    std::string path;
    path += origdir;
    path += '/';
    path += fname;

    nlohmann::json j;

    if (populateJson(path, j) != PHYSFS_ENUM_OK) {
        return PHYSFS_ENUM_OK;
    }

    nc::Tile* t = new nc::Tile;
    t->setName(j["name"].get<std::string>());
    t->setTexture(j["texture"].get<std::string>());
    if (j.contains("collidable")) {
        t->setCollidable(j["collidable"].get<bool>());
    }

    nc::Game::getInstance()->getRegistry().registerTile(t);

    return PHYSFS_ENUM_OK;
}

}

namespace nc {

Game* Game::m_inst = nullptr;

Game::Game(int argc, char** argv)
    : m_argc(argc), m_argv(argv), m_drawConsole(false), m_timeScale(1.0f),
      m_gameState(nullptr), m_requestedState(nullptr) {
    assert(m_inst == nullptr);
    m_inst = this;

    m_sink   = std::make_shared<spdlog::sinks::ostream_sink_mt>(m_logData);
    m_logger = std::make_shared<spdlog::logger>("nanolog", m_sink);
    m_logger->set_level(spdlog::level::trace);
    spdlog::set_default_logger(m_logger);
}

void Game::run() {
    setup();
    execute();
}

const nlohmann::json& Game::getSettings() const {
    return m_settings;
}

void Game::saveSettings() {
    std::ofstream o("settings.json");
    o << std::setw(4) << m_settings << std::endl;

    o.close();
}

TextureAtlas& Game::getTextureAtlas() {
    return m_atlas;
}

sf::View& Game::getView() {
    return m_view;
}

void Game::setTimeScale(const float scale) {
    m_timeScale = scale;
}

float Game::getTimeScale() const {
    return m_timeScale;
}

void Game::setState(GameState* newState) {
    m_requestedState = newState;
}

GameState* Game::getState() const {
    if (m_requestedState != nullptr) {
        return m_requestedState;
    }

    return m_gameState;
}

sf::RenderWindow& Game::getWindow() {
    return m_win;
}

GameRegistry& Game::getRegistry() {
    return m_reg;
}

Game* Game::getInstance() {
    return m_inst;
}

void Game::setup() {
    if (!PHYSFS_init(m_argv[0])) {
        throw std::runtime_error("Failed to initialize PHYSFS!");
    }

    sf::err().rdbuf(m_logData.rdbuf());

    spdlog::info("Nanocraft v{}.{}.{}.{}", NC_VER_MAJOR, NC_VER_MINOR,
                 NC_VER_PATCH, NC_VER_TWEAK);
    if (m_argc >= 2) {
        spdlog::info("Passed arguments:");
        for (int i = 1; i < m_argc; i++) {
            spdlog::info("\t{}", m_argv[i]);
        }
    }

#ifndef NC_DEBUG
    if (!std::filesystem::exists("data/base.zip") ||
        !std::filesystem::is_regular_file("data/base.zip")) {
        throw std::runtime_error(
            "No base game data file found! (data/base.zip)");
    }
    if (!PHYSFS_mount("data/base.zip", "/", 0)) {
        throw std::runtime_error(
            "Could not load base game data file! (data/base.zip)");
    }
#else
    if (!std::filesystem::exists("data/base") ||
        !std::filesystem::is_directory("data/base")) {
        throw std::runtime_error("Could not find base game directory!");
    }
    if (!PHYSFS_mount("data/base", "/", 0)) {
        throw std::runtime_error("Could not load base game directory!");
    }
#endif

    // Get settings from file if available
    if (std::filesystem::exists("settings.json")) {
        loadSettings();
    } else {
        createDefaultSettings();
        saveSettings();
    }

    // Create window
    auto modeWidth  = m_settings["display"]["resolution_x"].get<unsigned int>();
    auto modeHeight = m_settings["display"]["resolution_y"].get<unsigned int>();
    sf::Uint32 windowStyle =
        sf::Style::Titlebar | sf::Style::Close; // Default window style
    if (m_settings["display"]["window_type"] == "fullscreen") {
        windowStyle |= sf::Style::Fullscreen;
    } else if (m_settings["display"]["window_type"] == "borderless") {
        windowStyle = sf::Style::None;
    }

    float aspectRatio =
        static_cast<float>(modeWidth) / static_cast<float>(modeHeight);

    m_view.reset(sf::FloatRect(
        0.0f, 0.0f, static_cast<float>(Chunk::VIEWABLE_TILES),
        static_cast<float>(Chunk::VIEWABLE_TILES) / (aspectRatio)));

    m_win.create(sf::VideoMode(modeWidth, modeHeight), "Nanocraft",
                 windowStyle);
    m_win.setView(m_view);

    ImGui::SFML::Init(m_win);
    ImGui::GetIO().IniFilename = nullptr;
}

void Game::execute() {
    loadTextures();
    loadItems();
    loadTiles();

    // Get into the main menu
    setState(new MainMenuState());

    sf::Clock frameTime;
    sf::Clock updateFpsTimer;
    float fps   = 0.0f;
    float accum = 0.0f;

    while (m_win.isOpen()) {
        // Get delta time
        float elapsed = m_delta.restart().asSeconds();
        accum += elapsed;

        if (m_requestedState != nullptr) {
            delete m_gameState;
            m_gameState      = m_requestedState;
            m_requestedState = nullptr;
        }

        m_gameState->perFrame();

        // Process events
        sf::Event e;
        while (m_win.pollEvent(e)) {
            // Process imgui events
            ImGui::SFML::ProcessEvent(e);

            if (e.type == sf::Event::Closed) {
                m_win.close();
            } else if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == m_settings["controls"]["toggle_console"]
                                      .get<sf::Keyboard::Key>()) {
                    // Draw console with tilde
                    m_drawConsole = !m_drawConsole;
                }
            }

            m_gameState->handleEvent(e);
        }

        // Update imgui
        ImGui::SFML::Update(m_win, sf::seconds(elapsed));

        // Update state
        while (accum >= TIMESTEP) {
            m_gameState->update(TIMESTEP * m_timeScale);

            accum -= TIMESTEP;
        }

        // Draw gui here
        if (m_drawConsole) {
            ImGui::Begin("Console");
            std::string s = m_logData.str();
            ImGui::TextUnformatted(s.c_str());
            ImGui::End();
            // Draw performance window
            ImGui::Begin("Performance");
            ImGui::Text("FPS: %.2f", fps);
            ImGui::End();
        }

        // Draw
        m_win.setView(m_view);
        m_win.clear();
        m_gameState->draw(m_win);
        // Draw imgui
        ImGui::EndFrame();
        ImGui::SFML::Render(m_win);
        m_win.display();

        if (updateFpsTimer.getElapsedTime().asSeconds() >= 2.0f) {
            fps = 1.0f / frameTime.restart().asSeconds();
            updateFpsTimer.restart();
        } else {
            frameTime.restart();
        }
    }
}

void Game::loadSettings() {
    m_settings.clear();

    std::ifstream i("settings.json");
    i >> m_settings;

    i.close();
}

void Game::createDefaultSettings() {
    m_settings.clear();
    // Display settings
    m_settings["display"]["resolution_x"] = 1280;
    m_settings["display"]["resolution_y"] = 720;
    m_settings["display"]["window_type"]  = "window";
    // Control settings
    m_settings["controls"]["toggle_console"] = sf::Keyboard::Tilde;
    m_settings["controls"]["move_up"]        = sf::Keyboard::W;
    m_settings["controls"]["move_down"]      = sf::Keyboard::S;
    m_settings["controls"]["move_left"]      = sf::Keyboard::A;
    m_settings["controls"]["move_right"]     = sf::Keyboard::D;
    // Debug settings
    m_settings["debug"]["test_seed"] = 7582;
}

void Game::loadTextures() {
    PHYSFS_enumerate("/textures", loadTextureCallback, NULL);
}

void Game::loadItems() {
    PHYSFS_enumerate("/items", loadItemCallback, NULL);
}

void Game::loadTiles() {
    PHYSFS_enumerate("/tiles", loadTileCallback, NULL);
}

}