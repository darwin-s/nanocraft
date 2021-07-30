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

#ifndef NC_GAME_HPP
#define NC_GAME_HPP

#include <SFML/Graphics.hpp>

namespace nc {

class Game {
public:
    Game(int argc, char** argv);
    void run();
    static Game* getInstance();

private:
    void setup();
    void execute();

private:
    static Game* m_inst; // Game instance

    int m_argc;
    char** m_argv;

    sf::RenderWindow m_win; // Game window
};

}

#endif // !NC_GAME_HPP