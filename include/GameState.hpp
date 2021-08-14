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

#ifndef NC_GAMESTATE_HPP
#define NC_GAMESTATE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace nc {

class GameState {
public:
    virtual void perFrame()                  = 0;
    virtual void handleEvent(sf::Event e)    = 0;
    virtual void update(float dt)            = 0;
    virtual void draw(sf::RenderWindow& win) = 0;
};

}

#endif // !NC_GAMESTATE_HPP