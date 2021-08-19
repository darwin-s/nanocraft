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

Map::Map(Generator* gen)
    : m_gen(gen) {
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
    }
    m_chunks[y][x]->generateTexture();
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
}

Tile& Map::getTile(unsigned int xPos, unsigned int yPos) {
    sf::Vector2u cp = getChunkPos(xPos, yPos);
    Chunk* c = getChunk(cp);
    xPos -= cp.x * Chunk::CHUNK_SIZE;
    yPos -= cp.y * Chunk::CHUNK_SIZE;

    return c->getTile(xPos, yPos);
}

}