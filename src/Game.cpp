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
#include <iostream>
#include <cassert>

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

Game* Game::getInstance() {
    return m_inst;
}

void Game::setup() {
    std::cout << "nanocraft v" << NC_VER_MAJOR << "." << NC_VER_MINOR << "."
              << NC_VER_PATCH << "." << NC_VER_TWEAK << std::endl;
    if (m_argc >= 2) {
        std::cout << "Passed arguments:\n";
        for (int i = 1; i < m_argc; i++) {
            std::cout << "\t" << m_argv[i] << "\n";
        }
        std::cout << std::flush;
    }

    // TODO: get game settings
    m_win.create(sf::VideoMode(640, 480), "Nanocraft");
}

void Game::execute() {
    while(m_win.isOpen()) {
        // Process events
        sf::Event e;
        while(m_win.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                m_win.close();
            }
        }
    }
}

}