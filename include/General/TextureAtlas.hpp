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

#ifndef NC_GENERAL_TEXTUREATLAS_HPP
#define NC_GENERAL_TEXTUREATLAS_HPP

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <unordered_map>
#include <filesystem>
#include <string>

namespace nc {

class TextureAtlas {
public:
    explicit TextureAtlas();
    bool addTexture(const std::filesystem::path& path);
    const sf::Texture& getTexture(const std::string& texture) const;

private:
    std::unordered_map<std::string, sf::Texture> m_textures;
};

}

#endif // !NC_GENERAL_TEXTUREATLAS_HPP