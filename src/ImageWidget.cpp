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

#include <ImageWidget.hpp>
#include <Game.hpp>

namespace nc {

ImageWidget::ImageWidget(Widget* parent) : Widget(parent) {}

ImageWidget::ImageWidget(const std::string& texture, Widget* parent)
    : Widget(parent) {
    setTexture(texture);
}

void ImageWidget::setTexture(const std::string& texture) {
    TextureAtlas::TextureInfo inf =
        Game::getInstance()->getTextureAtlas().getTexture(texture);
    m_sprite.setTexture(*inf.texture);
    m_sprite.setTextureRect(inf.textureRect);
}

sf::Sprite& ImageWidget::getSprite() {
    return m_sprite;
}

void ImageWidget::handleEvent(sf::Event e) {}

void ImageWidget::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
    target.draw(m_sprite, states);
}

}