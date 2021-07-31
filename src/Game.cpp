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

#include <Game.hpp>
#include <Version.hpp>
#include <physfs.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <cassert>
#include <stdexcept>

namespace nc {

Game* Game::m_inst = nullptr;

Game::Game(int argc, char** argv) : m_argc(argc), m_argv(argv) {
    assert(m_inst == nullptr);
    m_inst = this;
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

Game* Game::getInstance() {
    return m_inst;
}

void Game::setup() {
    if (!PHYSFS_init(m_argv[0])) {
        throw std::runtime_error("Failed to initialize PHYSFS!");
    }

    std::cout << "Nanocraft v" << NC_VER_MAJOR << "." << NC_VER_MINOR << "."
              << NC_VER_PATCH << "." << NC_VER_TWEAK << std::endl;
    if (m_argc >= 2) {
        std::cout << "Passed arguments:\n";
        for (int i = 1; i < m_argc; i++) {
            std::cout << "\t" << m_argv[i] << "\n";
        }
        std::cout << std::flush;
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
    unsigned int modeWidth =
        m_settings["display"]["resolution_x"].get<unsigned int>();
    unsigned int modeHeight =
        m_settings["display"]["resolution_y"].get<unsigned int>();
    sf::Uint32 windowStyle =
        sf::Style::Titlebar | sf::Style::Close; // Default window style
    if (m_settings["display"]["window_type"] == "fullscreen") {
        windowStyle |= sf::Style::Fullscreen;
    } else if (m_settings["display"]["window_type"] == "borderless") {
        windowStyle = sf::Style::None;
    }

    m_win.create(sf::VideoMode(modeWidth, modeHeight), "Nanocraft",
                 windowStyle);
}

void Game::execute() {
    TextureAtlas ta;
    ta.addTexture("grass.png");
    ta.addTexture("sand.png");
    ta.addTexture("sky.png");

    TextureAtlas::TextureInfo grassTex = ta.getTexture("grass.png");
    TextureAtlas::TextureInfo sandTex = ta.getTexture("sand.png");
    TextureAtlas::TextureInfo skyTex = ta.getTexture("sky.png");

    sf::Sprite grass;
    sf::Sprite sand;
    sf::Sprite sky;

    grass.setTexture(*grassTex.texture);
    sand.setTexture(*sandTex.texture);
    sky.setTexture(*skyTex.texture);

    grass.setTextureRect(grassTex.textureRect);
    sand.setTextureRect(sandTex.textureRect);
    sky.setTextureRect(skyTex.textureRect);

    sand.move(32.0f, 0.0f);
    sky.move(64.0f, 0.0f);

    while (m_win.isOpen()) {
        // Process events
        sf::Event e;
        while (m_win.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                m_win.close();
            }
        }

        // Draw
        m_win.clear();
        m_win.draw(grass);
        m_win.draw(sand);
        m_win.draw(sky);
        m_win.display();
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
    m_settings["display"]["resolution_x"] = 640;
    m_settings["display"]["resolution_y"] = 480;
    m_settings["display"]["window_type"]  = "window";
}

}