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

#ifndef NC_TEXTUREATLAS_HPP
#define NC_TEXTUREATLAS_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include <unordered_map>
#include <filesystem>
#include <string>

namespace nc {

class TextureAtlas {
public:
    struct TextureInfo {
        sf::Texture* texture;
        sf::IntRect textureRect;
        unsigned int textureIndex; // Used by the texture atlas
    };

public:
    static constexpr unsigned int MAX_TEXTURES = 16;
    static constexpr unsigned int DEFAULT_TILE_SIZE = 32;

public:
    explicit TextureAtlas(unsigned int tileSize = DEFAULT_TILE_SIZE);
    bool addTexture(const std::filesystem::path& path);
    TextureInfo getTexture(const std::string& texture) const;

private:
    struct AtlasPage {
        sf::Texture tex;
        unsigned int xFreeOffset;
        unsigned int yFreeOffset;
        bool full;
    };

private:
    std::array<AtlasPage, MAX_TEXTURES> m_textures;
    std::unordered_map<std::string, TextureInfo> m_texInfo;
    const unsigned int m_tileSize;
};

}

#endif // !NC_TEXTUREATLAS_HPP