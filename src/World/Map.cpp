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

#include <World/Map.hpp>
#include <Components/PlayerComponent.hpp>
#include <Components/AnimationComponent.hpp>
#include <General/Object.hpp>
#include <random>
#include <array>

namespace nc {

sf::Vector2u Map::getChunkPos(float x, float y) {
    return sf::Vector2u(static_cast<unsigned int>(x / Chunk::CHUNK_SIZE),
                        static_cast<unsigned int>(y / Chunk::CHUNK_SIZE));
}

sf::Vector2u Map::getChunkPos(sf::Vector2f pos) {
    return getChunkPos(pos.x, pos.y);
}

sf::Vector2f Map::getGlobalPos(unsigned int chunkX, unsigned int chunkY,
                               unsigned int tileX, unsigned int tileY) {
    return sf::Vector2f(static_cast<float>(chunkX * Chunk::CHUNK_SIZE + tileX),
                        static_cast<float>(chunkY * Chunk::CHUNK_SIZE + tileY));
}

sf::Vector2f Map::getGlobalPos(sf::Vector2u chunkPos, sf::Vector2u tilePos) {
    return getGlobalPos(chunkPos.x, chunkPos.y, tilePos.x, tilePos.y);
}

Map::Map(Generator* gen) : m_gen(gen) {
    m_chunks = new Chunk**[CHUNK_NO];
    for (unsigned int y = 0; y < CHUNK_NO; y++) {
        m_chunks[y] = new Chunk*[CHUNK_NO];
        for (unsigned int x = 0; x < CHUNK_NO; x++) {
            m_chunks[y][x] = nullptr;
        }
    }
}

Map::~Map() {
    for (unsigned int y = 0; y < CHUNK_NO; y++) {
        for (unsigned int x = 0; x < CHUNK_NO; x++) {
            delete m_chunks[y][x];
        }
        delete[] m_chunks[y];
    }

    delete[] m_chunks;
}

void Map::setGenerator(Generator* gen) {
    m_gen = gen;
}

Generator* Map::getGenerator() const {
    return m_gen;
}

Chunk* Map::getChunk(const unsigned int x, const unsigned int y) {
    return m_chunks[y][x];
}

Chunk* Map::getChunk(const sf::Vector2u pos) {
    return getChunk(pos.x, pos.y);
}

void Map::generateChunk(unsigned int x, unsigned int y) {
    m_chunks[y][x] = new Chunk(x, y);
    if (m_gen != nullptr) {
        m_gen->generateChunk(m_chunks[y][x]);
        for (unsigned int tileY = 0; tileY < Chunk::CHUNK_SIZE; tileY++) {
            for (unsigned int tileX = 0; tileX < Chunk::CHUNK_SIZE; tileX++) {
                updateTile(x * Chunk::CHUNK_SIZE + tileX,
                           y * Chunk::CHUNK_SIZE + tileY);
            }
        }

        // Update neigbouring edge tiles
        Chunk* top = nullptr;
        if (y != 0) {
            top = getChunk(x, y - 1);
        }

        Chunk* bottom = nullptr;
        if (y != CHUNK_NO - 1) {
            bottom = getChunk(x, y + 1);
        }

        Chunk* left = nullptr;
        if (x != 0) {
            left = getChunk(x - 1, y);
        }

        Chunk* right = nullptr;
        if (x != CHUNK_NO - 1) {
            right = getChunk(x + 1, y);
        }

        // Top neighbour
        if (top != nullptr) {
            const unsigned int tileY = Chunk::CHUNK_SIZE - 1;
            for (unsigned int tileX = 0; tileX < Chunk::CHUNK_SIZE; tileX++) {
                sf::Vector2u globalPos = static_cast<sf::Vector2u>(
                    getGlobalPos(x, y - 1, tileX, tileY));
                top->getTile(tileX, tileY)
                    .update(globalPos.x, globalPos.y, this);
            }
        }

        // Bottom neighbour
        if (bottom != nullptr) {
            const unsigned int tileY = 0;
            for (unsigned int tileX = 0; tileX < Chunk::CHUNK_SIZE; tileX++) {
                sf::Vector2u globalPos = static_cast<sf::Vector2u>(
                    getGlobalPos(x, y + 1, tileX, tileY));
                bottom->getTile(tileX, tileY)
                    .update(globalPos.x, globalPos.y, this);
            }
        }

        // Left neighbour
        if (left != nullptr) {
            const unsigned int tileX = Chunk::CHUNK_SIZE - 1;
            for (unsigned int tileY = 0; tileY < Chunk::CHUNK_SIZE; tileY++) {
                sf::Vector2u globalPos = static_cast<sf::Vector2u>(
                    getGlobalPos(x - 1, y, tileX, tileY));
                left->getTile(tileX, tileY)
                    .update(globalPos.x, globalPos.y, this);
            }
        }

        // Right neighbour
        if (right != nullptr) {
            const unsigned int tileX = 0;
            for (unsigned int tileY = 0; tileY < Chunk::CHUNK_SIZE; tileY++) {
                sf::Vector2u globalPos = static_cast<sf::Vector2u>(
                    getGlobalPos(x + 1, y, tileX, tileY));
                right->getTile(tileX, tileY)
                    .update(globalPos.x, globalPos.y, this);
            }
        }
    }
}

void Map::generateChunk(const sf::Vector2u pos) {
    generateChunk(pos.x, pos.y);
}

entt::registry& Map::getRegistry() {
    return m_reg;
}

void Map::simulateWorld(const float dt) {
    // Generate chunks around players
    m_reg.view<PlayerComponent, Object>().each([=](auto& obj) {
        constexpr int xDir[] = {0, -1, 0, 1, -1, 1, -1, 0, 1};
        constexpr int yDir[] = {0, -1, -1, -1, 0, 0, 1, 1, 1};

        for (int i = 0; i < 9; i++) {
            const unsigned int xPos =
                getChunkPos(obj.getPosition()).x + xDir[i];
            const unsigned int yPos =
                getChunkPos(obj.getPosition()).y + yDir[i];

            if (getChunk(xPos, yPos) == nullptr) {
                generateChunk(xPos, yPos);
            }
        }
    });

    // Update animations
    m_reg.view<AnimationComponent>().each([=](auto& ac) {
        ac.update(dt);
    });
}

void Map::placeTile(Tile* tile, unsigned int xPos, unsigned int yPos) {
    sf::Vector2u cp           = getChunkPos(xPos, yPos);
    Chunk* c                  = getChunk(cp);
    const unsigned int chunkX = xPos - cp.x * Chunk::CHUNK_SIZE;
    const unsigned int chunkY = yPos - cp.y * Chunk::CHUNK_SIZE;

    if (c != nullptr) {
        c->setTile(tile, chunkX, chunkY);
        updateTile(xPos, yPos);
    }
}

Tile* Map::getTile(unsigned int xPos, unsigned int yPos) {
    sf::Vector2u cp = getChunkPos(xPos, yPos);
    Chunk* c        = getChunk(cp);
    xPos -= cp.x * Chunk::CHUNK_SIZE;
    yPos -= cp.y * Chunk::CHUNK_SIZE;

    return c == nullptr ? nullptr : &c->getTile(xPos, yPos);
}

void Map::updateTile(unsigned int tileX, unsigned int tileY) {
    sf::Vector2u cp           = getChunkPos(tileX, tileY);
    Chunk* c                  = getChunk(cp);
    const unsigned int chunkX = tileX - cp.x * Chunk::CHUNK_SIZE;
    const unsigned int chunkY = tileY - cp.y * Chunk::CHUNK_SIZE;

    if (c != nullptr) {
        c->getTile(chunkX, chunkY).update(tileX, tileY, this);
        c->setDirty();
        constexpr int xDir[] = {0, 0, -1, 1};
        constexpr int yDir[] = {-1, 1, 0, 0};

        for (unsigned int i = 0; i < 4; i++) {
            Tile* n = getTile(tileX + xDir[i], tileY + yDir[i]);

            if (n != nullptr) {
                n->update(tileX + xDir[i], tileY + yDir[i], this);
            }
        }
    }
}

}