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

#include <Map.hpp>
#include <random>

namespace nc {

Map::Map() : m_seed(std::default_random_engine::default_seed) {
    m_noiseGen.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    m_noiseGen.SetFractalType(FastNoiseLite::FractalType_FBm);
    m_noiseGen.SetSeed(m_seed);
    m_noiseGen.SetFrequency(FREQ);
    m_noiseGen.SetFractalOctaves(OCTAVES);
    m_chunks = new Chunk**[CHUNK_NO];
    for (unsigned int y = 0; y < CHUNK_NO; y++) {
        m_chunks[y] = new Chunk*[CHUNK_NO];
        for (unsigned int x = 0; x < CHUNK_NO; x++) {
            m_chunks[y][x] = nullptr;
        }
    }
}

Map::Map(std::uint32_t seed) : m_seed(seed) {
    m_noiseGen.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    m_noiseGen.SetFractalType(FastNoiseLite::FractalType_FBm);
    m_noiseGen.SetSeed(m_seed);
    m_noiseGen.SetFrequency(FREQ);
    m_noiseGen.SetFractalOctaves(OCTAVES);
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

Chunk* Map::getChunk(const unsigned int x, const unsigned int y) {
    return m_chunks[y][x];
}

void Map::generateChunk(unsigned int x, unsigned int y) {
    m_chunks[y][x] = new Chunk(x, y);
    for (unsigned int yTile = 0; yTile < Chunk::CHUNK_SIZE; yTile++) {
        for (unsigned int xTile = 0; xTile < Chunk::CHUNK_SIZE; xTile++) {
            const float globalXpos = x * Chunk::CHUNK_SIZE + xTile;
            const float globalYpos = y * Chunk::CHUNK_SIZE + yTile;
            const float val        = m_noiseGen.GetNoise(globalXpos, globalYpos);
            if (val < 0.0f) {
                m_chunks[y][x]->getTile(xTile, yTile).setTexture("sand.png");
            } else {
                m_chunks[y][x]->getTile(xTile, yTile).setTexture("grass.png");
            }
        }
    }
}

std::uint32_t Map::getSeed() const {
    return m_seed;
}

}