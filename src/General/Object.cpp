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

#include <General/Object.hpp>
#include <Game/Game.hpp>

namespace nc {

Object::Object(sf::Vector2u size) : m_size(size), sf::Sprite() {
    setTexture("default");
    setSize(m_size);
}

Object::Object(const std::string& texture, sf::Vector2u size)
    : m_size(size), sf::Sprite() {
    setTexture(texture);
    setSize(m_size);
}

void Object::setTexture(const std::string& texture) {
    sf::Sprite::setTexture(
        Game::getInstance()->getTextureAtlas().getTexture(texture));
}

sf::Vector2u Object::getSize() const {
    return m_size;
}

void Object::setSize(sf::Vector2u size) {
    m_size = size;
    const float texWidth =
        static_cast<float>(sf::Sprite::getTextureRect().width);
    const float texHeight =
        static_cast<float>(sf::Sprite::getTextureRect().height);

    sf::Sprite::setScale(
        static_cast<float>(size.x) / static_cast<float>(texWidth),
        static_cast<float>(size.y) / static_cast<float>(texHeight));
}

}
