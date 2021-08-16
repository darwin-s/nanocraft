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
#include <Game.hpp>
#include <Map.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace nc {

Chunk::Chunk(const unsigned int xPos, const unsigned int yPos)
    : m_xPos(xPos), m_yPos(yPos) {
    const unsigned int tileSize =
        Game::getInstance()->getTextureAtlas().getTileSize();
    m_tex.create(CHUNK_SIZE * tileSize, CHUNK_SIZE * tileSize);
    m_sprite.setScale(1.0f / static_cast<float>(tileSize),
                      1.0f / static_cast<float>(tileSize));
    m_sprite.setPosition(Map::getGlobalPos(xPos, yPos));
    m_sprite.setTexture(m_tex.getTexture());

    for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
        for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
            m_tiles[y][x].setPosition(static_cast<float>(x * tileSize),
                                      static_cast<float>(y * tileSize));
        }
    }
}

Tile& Chunk::getTile(const unsigned int x, const unsigned int y) {
    return m_tiles[y][x];
}

void Chunk::generateTexture() {
    m_tex.clear(sf::Color::Yellow);
    
    for (auto& tileRow : m_tiles) {
        for (auto& tile : tileRow) {
            m_tex.draw(tile);
        }
    }
    
    m_tex.display();
}

sf::Vector2u Chunk::getPosition() const {
    return sf::Vector2u(m_xPos, m_yPos);
}

void Chunk::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_sprite);
}

}
