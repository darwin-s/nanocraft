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

#ifndef NANOCRAFT_MAP_HPP
#define NANOCRAFT_MAP_HPP

#include <Chunk.hpp>
#include <SFML/System/Vector2.hpp>
#include <FastNoiseLite.h>
#include <entt/entt.hpp>

namespace nc {

class Map {
public:
    static constexpr unsigned int CHUNK_NO    = 1024;
    static constexpr float FREQ               = 0.04f;
    static constexpr unsigned long WORLD_SIZE = CHUNK_NO * Chunk::CHUNK_SIZE;
    static constexpr unsigned int OCTAVES     = 8;

public:
    static sf::Vector2u getChunkPos(float x, float y);
    static sf::Vector2u getChunkPos(sf::Vector2f pos);
    static sf::Vector2f getGlobalPos(unsigned int chunkX, unsigned int chunkY,
                                     unsigned int tileX = 0,
                                     unsigned int tileY = 0);
    static sf::Vector2f getGlobalPos(sf::Vector2u chunkPos,
                                     sf::Vector2u tilePos = sf::Vector2u(0, 0));

public:
    Map();
    explicit Map(std::uint32_t seed);
    ~Map();
    Chunk* getChunk(unsigned int x, unsigned int y);
    Chunk* getChunk(sf::Vector2u pos);
    void generateChunk(unsigned int x, unsigned int y);
    void generateChunk(sf::Vector2u pos);
    [[nodiscard]] std::uint32_t getSeed() const;
    entt::registry& getRegistry();
    void simulateWorld([[maybe_unused]] float dt);

private:
    Chunk*** m_chunks;
    FastNoiseLite m_noiseGen;
    std::uint32_t m_seed;
    entt::registry m_reg;
};

}

#endif // !NANOCRAFT_MAP_HPP
