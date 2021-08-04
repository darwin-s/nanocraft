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
#include <imgui.h>
#include <imgui-SFML.h>
#include <physfs.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <cassert>
#include <stdexcept>

namespace nc {

Game* Game::m_inst = nullptr;

Game::Game(int argc, char** argv)
    : m_argc(argc), m_argv(argv), m_drawConsole(false) {
    assert(m_inst == nullptr);
    m_inst = this;

    m_sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(m_logData);
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

Game* Game::getInstance() {
    return m_inst;
}

void Game::setup() {
    if (!PHYSFS_init(m_argv[0])) {
        throw std::runtime_error("Failed to initialize PHYSFS!");
    }

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

    ImGui::SFML::Init(m_win);
    ImGui::GetIO().IniFilename = nullptr;
}

void Game::execute() {
    TextureAtlas ta;
    ta.addTexture("grass.png");
    ta.addTexture("sand.png");
    ta.addTexture("skys.png");

    TextureAtlas::TextureInfo grassTex = ta.getTexture("grass.png");
    TextureAtlas::TextureInfo sandTex  = ta.getTexture("sand.png");
    TextureAtlas::TextureInfo skyTex   = ta.getTexture("skys.png");

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
            // Process imgui events
            ImGui::SFML::ProcessEvent(e);

            if (e.type == sf::Event::Closed) {
                m_win.close();
            } else if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Tilde) {
                    // Draw console with tide
                    m_drawConsole = !m_drawConsole;
                }
            }
        }

        ImGui::SFML::Update(m_win, m_delta.restart()); // Update imgui
        // Draw gui here
        if (m_drawConsole) {
            ImGui::Begin("Console");
            std::string s = m_logData.str();
            ImGui::TextUnformatted(s.c_str());
            ImGui::End();
        }

        // Draw
        m_win.clear();
        m_win.draw(grass);
        m_win.draw(sand);
        m_win.draw(sky);
        // Draw imgui
        ImGui::SFML::Render(m_win);
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