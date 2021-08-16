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
#include <Game/Game.hpp>

namespace nc {

Tile::Tile() : sf::Sprite() {
    setTexture("default");
}

Tile::Tile(const std::string& texture) : sf::Sprite() {
    setTexture(texture);
}

void Tile::setTexture(const std::string& texture) {
    TextureAtlas::TextureInfo inf = Game::getInstance()->getTextureAtlas().getTexture(texture);
    sf::Sprite::setTexture(*inf.texture);
    sf::Sprite::setTextureRect(inf.textureRect);
    m_size = Game::getInstance()->getTextureAtlas().getTileSize();
}

unsigned int Tile::getSize() const {
    return m_size;
}

}