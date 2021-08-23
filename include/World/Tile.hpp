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

#ifndef NC_WORLD_TILE_HPP
#define NC_WORLD_TILE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <string>

namespace nc {

class Map;

class Tile : public sf::Sprite {
public:
    explicit Tile(const std::string& name = "null");
    explicit Tile(const std::string& texture, const std::string& name);
    void setTexture(const std::string& texture);
    unsigned int getSize() const;
    void update(unsigned int posX, unsigned int posY, Map* currentMap);
    void setName(const std::string& name);
    std::string getName() const;

private:
    unsigned int m_size;
    std::string m_name;
    static sf::IntRect m_textureRects[16];
};

}

#endif // !NC_WORLD_TILE_HPP
