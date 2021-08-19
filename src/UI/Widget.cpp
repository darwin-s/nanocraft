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

#include <UI/Widget.hpp>
#include <Game/Game.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace nc {

Widget::Widget(Widget* parent)
    : m_parent(parent), m_focused(false), m_shown(true), m_ui(nullptr) {}

Widget* Widget::getParent() {
    return m_parent;
}

void Widget::setParent(Widget* parent) {
    m_parent = parent;
}

bool Widget::getFocused() const {
    return m_focused;
}

void Widget::setShown(bool show) {
    m_shown = show;
}

bool Widget::getShown() const {
    return m_shown;
}

sf::Sprite& Widget::getSprite() {
    return m_sprite;
}

void Widget::setSize(float width, float height) {
    sf::IntRect texRect = m_sprite.getTextureRect();
    m_sprite.setScale(width / static_cast<float>(texRect.width),
                      height / static_cast<float>(texRect.height));
}

void Widget::setSize(sf::Vector2f size) {
    setSize(size.x, size.y);
}

sf::Vector2f Widget::getSize() const {
    const sf::IntRect& texRect = m_sprite.getTextureRect();
    sf::Vector2f s{static_cast<float>(texRect.width),
                   static_cast<float>(texRect.height)};

    s.x *= m_sprite.getScale().x;
    s.y *= m_sprite.getScale().y;

    return s;
}

void Widget::setPosition(float x, float y) {
    if (m_parent == nullptr) {
        m_sprite.setPosition(x, y);
    } else {
        m_sprite.setPosition(m_parent->m_sprite.getPosition().x + x,
                             m_parent->m_sprite.getPosition().y + y);
    }
}

void Widget::setPosition(sf::Vector2f pos) {
    setPosition(pos.x, pos.y);
}

sf::Vector2f Widget::getPosition() const {
    return m_sprite.getPosition();
}

void Widget::setTexture(const std::string& texture) {
    TextureAtlas::TextureInfo inf =
        Game::getInstance()->getTextureAtlas().getTexture(texture);

    m_sprite.setTexture(*inf.texture);
    m_sprite.setTextureRect(inf.textureRect);
}

void Widget::setFocused(bool focus) {
    m_focused = focus;
}

UI* Widget::getUI() {
    return m_ui;
}

void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (m_shown) {
        target.draw(m_sprite, states);
    }
}

}