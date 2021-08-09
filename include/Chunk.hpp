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

#ifndef NANOCRAFT_CHUNK_HPP
#define NANOCRAFT_CHUNK_HPP

#include <Tile.hpp>
#include <SFML/Graphics/Drawable.hpp>

namespace nc {

class Chunk : public sf::Drawable {
public:
    static constexpr unsigned int CHUNK_SIZE = 32;

public:
    Chunk(unsigned int xPos, unsigned int yPos);
    Tile& getTile(unsigned int x, unsigned  int y);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    Tile m_tiles[CHUNK_SIZE][CHUNK_SIZE];
    unsigned int m_xPos;
    unsigned int m_yPos;
};

}

#endif // !NANOCRAFT_CHUNK_HPP
