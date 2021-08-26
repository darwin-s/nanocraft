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

#include <World/OverworldGenerator.hpp>
#include <Game/Game.hpp>
#include <random>

namespace nc {

OverworldGenerator::OverworldGenerator()
    : Generator(std::default_random_engine::default_seed) {
    m_noiseGen.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    m_noiseGen.SetFractalType(FastNoiseLite::FractalType_FBm);
    m_noiseGen.SetSeed(getSeed());
    m_noiseGen.SetFrequency(FREQ);
    m_noiseGen.SetFractalOctaves(OCTAVES);
}

OverworldGenerator::OverworldGenerator(std::uint32_t seed) : Generator(seed) {
    m_noiseGen.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    m_noiseGen.SetFractalType(FastNoiseLite::FractalType_FBm);
    m_noiseGen.SetSeed(getSeed());
    m_noiseGen.SetFrequency(FREQ);
    m_noiseGen.SetFractalOctaves(OCTAVES);
}

void OverworldGenerator::generateChunk(Chunk* chunk) const {
    sf::Vector2u pos = chunk->getPosition();
    for (unsigned int yTile = 0; yTile < Chunk::CHUNK_SIZE; yTile++) {
        for (unsigned int xTile = 0; xTile < Chunk::CHUNK_SIZE; xTile++) {
            const float globalXpos = pos.x * Chunk::CHUNK_SIZE + xTile;
            const float globalYpos = pos.y * Chunk::CHUNK_SIZE + yTile;
            const float val = m_noiseGen.GetNoise(globalXpos, globalYpos);
            makeLandscape(xTile, yTile, val, chunk);
        }
    }
}

void OverworldGenerator::makeLandscape(unsigned int x, unsigned int y,
                                       float noiseVal, Chunk* chunk) const {
    if (noiseVal < 0.0f) {
        chunk->setTile(Game::getInstance()->getRegistry().getTile("sand"), x, y);
    } else {
        chunk->setTile(Game::getInstance()->getRegistry().getTile("grass"), x, y);
    }
}

}