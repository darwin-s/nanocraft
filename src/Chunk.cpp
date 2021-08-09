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

#include <Chunk.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace nc {

Chunk::Chunk(const unsigned int xPos, const unsigned int yPos)
    : m_xPos(xPos), m_yPos(yPos) {
    for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
        for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
            m_tiles[y][x].setPosition(static_cast<float>(m_xPos * CHUNK_SIZE + x), static_cast<float>(m_yPos * CHUNK_SIZE + y));
        }
    }
}

Tile& Chunk::getTile(const unsigned int x, const unsigned int y) {
    return m_tiles[y][x];
}

void Chunk::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& tileRow : m_tiles) {
        for (const auto& tile : tileRow) {
            target.draw(tile, states);
        }
    }
}

}
