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

#include <Game/Item.hpp>
#include <Game/Game.hpp>

namespace nc {

Item::Item(const std::string& name) : m_name(name), m_placeableTile("null") {
    setTexture("default");
}

Item::Item(const std::string& texture, const std::string& name)
    : m_name(name), m_placeableTile("null") {
    setTexture(texture);
}

void Item::setTexture(const std::string& texture) {
    m_sprite.setTexture(
        Game::getInstance()->getTextureAtlas().getTexture(texture));
    m_sprite.setScale(
        1.0f / static_cast<float>(m_sprite.getTextureRect().width),
        1.0f /
            static_cast<float>(m_sprite.getTextureRect().height));
}

sf::Sprite& Item::getSprite() {
    return m_sprite;
}

const sf::Sprite& Item::getSprite() const {
    return m_sprite;
}

void Item::setPlaceableTile(const std::string& name) {
    m_placeableTile = name;
}

Tile* Item::getPlaceableTile() const {
    return nullptr;
}

std::string Item::getName() const {
    return m_name;
}

void Item::setName(const std::string& name) {
    m_name = name;
}

}