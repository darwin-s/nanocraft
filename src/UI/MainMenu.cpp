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

#include <UI/MainMenu.hpp>
#include <Game/Game.hpp>
#include <Game/PlayingState.hpp>
#include <algorithm>

namespace nc {

MainMenu::MainMenu()
    : m_background("menubackground.png"), m_startGame("button.png") {
    addWidget(&m_background);
    addWidget(&m_startGame);

    m_background.setSize(UI::REFERENCE_WIDTH, UI::REFERENCE_HEIGHT);
    m_startGame.setPosition(0.4f * UI::REFERENCE_WIDTH,
                            0.55f * UI::REFERENCE_HEIGHT);
    m_startGame.setSize(0.2f * UI::REFERENCE_WIDTH,
                        0.1f * UI::REFERENCE_HEIGHT);
    m_startGame.setOnClick(
        []() { Game::getInstance()->setState(new PlayingState()); });
}

void MainMenu::update() {
    
}

}