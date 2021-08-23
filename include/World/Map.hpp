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

#ifndef NC_WORLD_MAP_HPP
#define NC_WORLD_MAP_HPP

#include <World/Chunk.hpp>
#include <World/Generator.hpp>
#include <SFML/System/Vector2.hpp>
#include <entt/entt.hpp>

namespace nc {

class Map {
public:
    static constexpr unsigned int CHUNK_NO    = 1024;

public:
    static sf::Vector2u getChunkPos(float x, float y);
    static sf::Vector2u getChunkPos(sf::Vector2f pos);
    static sf::Vector2f getGlobalPos(unsigned int chunkX, unsigned int chunkY,
                                     unsigned int tileX = 0,
                                     unsigned int tileY = 0);
    static sf::Vector2f getGlobalPos(sf::Vector2u chunkPos,
                                     sf::Vector2u tilePos = sf::Vector2u(0, 0));

public:
    explicit Map(Generator* gen = nullptr);
    ~Map();
    void setGenerator(Generator* gen);
    Generator* getGenerator() const;
    Chunk* getChunk(unsigned int x, unsigned int y);
    Chunk* getChunk(sf::Vector2u pos);
    void generateChunk(unsigned int x, unsigned int y);
    void generateChunk(sf::Vector2u pos);
    entt::registry& getRegistry();
    void simulateWorld([[maybe_unused]] float dt);
    Tile* getTile(unsigned int xPos, unsigned int yPos);
    void updateTile(unsigned int tileX, unsigned int tileY);

private:
    Chunk*** m_chunks;
    entt::registry m_reg;
    Generator* m_gen;
};

}

#endif // !NC_WORLD_MAP_HPP
