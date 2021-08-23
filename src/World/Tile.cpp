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

#include <World/Tile.hpp>
#include <World/Map.hpp>
#include <Game/Game.hpp>

namespace {

constexpr unsigned int getIndexFromConnections(bool up, bool down, bool left,
                                               bool right) {
    unsigned int res = 0;

    if (up) {
        res |= 0b1000;
    }

    if (down) {
        res |= 0b0100;
    }

    if (left) {
        res |= 0b0010;
    }

    if (right) {
        res |= 0b0001;
    }

    return res;
}

}

namespace nc {

sf::IntRect Tile::m_textureRects[16] = {
    sf::IntRect(48, 48, 16, 16), // up = 0, down = 0, left = 0, right = 0
    sf::IntRect(0, 48, 16, 16),  // up = 0, down = 0, left = 0, right = 1
    sf::IntRect(32, 48, 16, 16), // up = 0, down = 0, left = 1, right = 0
    sf::IntRect(16, 48, 16, 16), // up = 0, down = 0, left = 1, right = 1
    sf::IntRect(48, 0, 16, 16),  // up = 0, down = 1, left = 0, right = 0
    sf::IntRect(0, 0, 16, 16),   // up = 0, down = 1, left = 0, right = 1
    sf::IntRect(32, 0, 16, 16),  // up = 0, down = 1, left = 1, right = 0
    sf::IntRect(16, 0, 16, 16),  // up = 0, down = 1, left = 1, right = 1
    sf::IntRect(48, 32, 16, 16), // up = 1, down = 0, left = 0, right = 0
    sf::IntRect(0, 32, 16, 16),  // up = 1, down = 0, left = 0, right = 1
    sf::IntRect(32, 32, 16, 16), // up = 1, down = 0, left = 1, right = 0
    sf::IntRect(16, 32, 16, 16), // up = 1, down = 0, left = 1, right = 1
    sf::IntRect(48, 16, 16, 16), // up = 1, down = 1, left = 0, right = 0
    sf::IntRect(0, 16, 16, 16),  // up = 1, down = 1, left = 0, right = 1
    sf::IntRect(32, 16, 16, 16), // up = 1, down = 1, left = 1, right = 0
    sf::IntRect(16, 16, 16, 16)  // up = 1, down = 1, left = 1, right = 1
};

Tile::Tile(const std::string& name) : sf::Sprite(), m_name(name) {
    setTexture("default");
}

Tile::Tile(const std::string& texture, const std::string& name)
    : sf::Sprite(), m_name(name) {
    setTexture(texture);
}

void Tile::setTexture(const std::string& texture) {
    sf::Sprite::setTexture(
        Game::getInstance()->getTextureAtlas().getTexture(texture));
    sf::Sprite::setTextureRect(sf::IntRect(16, 16, 16, 16));
    m_size = sf::Sprite::getTextureRect().width;
}

unsigned int Tile::getSize() const {
    return m_size;
}

void Tile::update(unsigned int posX, unsigned int posY, Map* currentMap) {
    constexpr int xDir[] = {0, 0, -1, 1};
    constexpr int yDir[] = {-1, 1, 0, 0};
    bool connected[]     = {false, false, false, false};

    for (unsigned int i = 0; i < 4; i++) {
        sf::Vector2u chunkPos =
            Map::getChunkPos(posX + xDir[i], posY + yDir[i]);
        Chunk* c = currentMap->getChunk(chunkPos);

        Tile* n = currentMap->getTile(posX + xDir[i], posY + yDir[i]);

        if (n != nullptr && n->getTexture() == sf::Sprite::getTexture()) {
            connected[i] = true;
            c->setDirty();
        }
    }

    unsigned int idx = getIndexFromConnections(connected[0], connected[1],
                                               connected[2], connected[3]);

    sf::Sprite::setTextureRect(m_textureRects[idx]);
}

std::string Tile::getName() const {
    return m_name;
}

void Tile::setName(const std::string& name) {
    m_name = name;
}

}
