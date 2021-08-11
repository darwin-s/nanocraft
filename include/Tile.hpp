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

#ifndef NANOCRAFT_TILE_HPP
#define NANOCRAFT_TILE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <string>

namespace nc {

class Tile : public sf::Sprite {
public:
    Tile();
    explicit Tile(const std::string& texture);
    void setTexture(const std::string& texture);
    unsigned int getSize() const;

private:
    unsigned int m_size;
};

}

#endif // !NANOCRAFT_TILE_HPP
