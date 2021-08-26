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

#include <World/Chunk.hpp>
#include <Game/Game.hpp>
#include <World/Map.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace nc {

Chunk::Chunk(const unsigned int xPos, const unsigned int yPos)
    : m_xPos(xPos), m_yPos(yPos), m_dirty(true) {
    m_tex.create(CHUNK_SIZE * TextureAtlas::TILE_SIZE,
                 CHUNK_SIZE * TextureAtlas::TILE_SIZE);
    m_sprite.setScale(1.0f / static_cast<float>(TextureAtlas::TILE_SIZE),
                      1.0f / static_cast<float>(TextureAtlas::TILE_SIZE));
    m_sprite.setPosition(Map::getGlobalPos(xPos, yPos));
    m_sprite.setTexture(m_tex.getTexture());

    for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
        for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
            m_tiles[y][x].setPosition(
                static_cast<float>(x * TextureAtlas::TILE_SIZE),
                static_cast<float>(y * TextureAtlas::TILE_SIZE));
        }
    }
}

void Chunk::setTile(Tile* tile, unsigned int xPos, unsigned int yPos) {
    m_tiles[yPos][xPos] = *tile;
    m_tiles[yPos][xPos].setPosition(
            static_cast<float>(xPos * TextureAtlas::TILE_SIZE),
            static_cast<float>(yPos * TextureAtlas::TILE_SIZE));
    setDirty();

    if (tile->isCollidable()) {
        sf::Vector2f globalPos = Map::getGlobalPos(m_xPos, m_yPos, xPos, yPos);
        m_tiles[yPos][xPos].setCollisionBox(
            sf::FloatRect(globalPos.x, globalPos.y, 1.0f, 1.0f));
    }
}

void Chunk::setTile(Tile* tile, sf::Vector2u pos) {
    setTile(tile, pos.x, pos.y);
}

Tile& Chunk::getTile(const unsigned int x, const unsigned int y) {
    return m_tiles[y][x];
}

Tile& Chunk::getTile(sf::Vector2u pos) {
    return getTile(pos.x, pos.y);
}

void Chunk::setDirty() {
    m_dirty = true;
}

sf::Vector2u Chunk::getPosition() const {
    return sf::Vector2u(m_xPos, m_yPos);
}

void Chunk::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (m_dirty) {
        m_tex.clear(sf::Color::Yellow);

        for (const auto& tileRow : m_tiles) {
            for (const auto& tile : tileRow) {
                m_tex.draw(tile);
            }
        }

        m_tex.display();

        m_dirty = false;
    }

    target.draw(m_sprite);
}

}
