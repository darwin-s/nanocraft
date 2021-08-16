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

#include <UI/ButtonWidget.hpp>
#include <Game/Game.hpp>
#include <UI/UI.hpp>
#include <spdlog/spdlog.h>

namespace nc {

ButtonWidget::ButtonWidget(Widget* parent) : Widget(parent) {}

ButtonWidget::ButtonWidget(const std::string& texture, Widget* parent)
    : Widget(parent) {
    setTexture(texture);
}

void ButtonWidget::setOnClick(std::function<void()> func) {
    m_onClick = func;
}

void ButtonWidget::handleEvent(sf::Event e) {
    if (e.type == sf::Event::MouseButtonPressed) {
        if (e.mouseButton.button == sf::Mouse::Left) {
            sf::RenderWindow& win = Game::getInstance()->getWindow();
            UI* ui = getUI();
            sf::Vector2i mousePos(e.mouseButton.x, e.mouseButton.y);
            sf::Vector2f pos = win.mapPixelToCoords(mousePos, ui->getView());

            if (getSprite().getGlobalBounds().contains(pos)) {
                m_onClick();
            }
        }
    }
}

}