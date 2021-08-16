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

#ifndef NC_GAME_MAINMENUSTATE_HPP
#define NC_GAME_MAINMENUSTATE_HPP

#include <Game/GameState.hpp>
#include <UI/MainMenu.hpp>

namespace nc {

class MainMenuState : public GameState {
public:
    MainMenuState();
    void perFrame() override;
    void handleEvent(sf::Event e) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& win) override;

private:
    MainMenu m_mainMenu;
};

}

#endif // !NC_GAME_MAINMENUSTATE_HPP