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

#include <TextureAtlas.hpp>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/ostream_sink.h>
#include <sstream>
#include <memory>

namespace nc {

class Game {
public:
    Game(int argc, char** argv);
    void run();
    const nlohmann::json& getSettings() const;
    void saveSettings();

    static Game* getInstance();

private:
    void setup();
    void execute();
    void loadSettings();
    void createDefaultSettings();

private:
    static Game* m_inst; // Game instance

    int m_argc;
    char** m_argv;
    nlohmann::json m_settings;
    sf::Clock m_delta; // Delta time clock
    bool m_drawConsole;
    std::ostringstream m_logData; // Log data stream
    std::shared_ptr<spdlog::sinks::ostream_sink_mt> m_sink; // Log sink
    std::shared_ptr<spdlog::logger> m_logger; // Logger

    sf::RenderWindow m_win; // Game window
};

}

#endif // !NC_GAME_HPP