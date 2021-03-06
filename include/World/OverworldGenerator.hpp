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

#ifndef NC_WORLD_OVERWORLDGENERATOR_HPP
#define NC_WORLD_OVERWORLDGENERATOR_HPP

#include <World/Generator.hpp>
#include <FastNoiseLite.h>

namespace nc {

class OverworldGenerator : public Generator {
public:
    static constexpr float FREQ           = 0.02f;
    static constexpr unsigned int OCTAVES = 2;

public:
    OverworldGenerator();
    explicit OverworldGenerator(std::uint32_t seed);

    void generateChunk(Chunk* chunk) const override;

private:
    void makeLandscape(unsigned int x, unsigned int y, float noiseVal,
                       Chunk* chunk) const;

private:
    mutable FastNoiseLite m_noiseGen;
};

}

#endif // !NC_WORLD_OVERWORLDGENERATOR_HPP