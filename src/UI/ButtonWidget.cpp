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

ButtonWidget::ButtonWidget(Widget* parent) : Widget(parent), m_state(NORMAL) {
    for (int s = 0; s < STATE_NO; s++) {
        setTexture(static_cast<State>(s), "default");
    }

    Widget::setTexture("default");
}

ButtonWidget::ButtonWidget(const std::string& texture, Widget* parent)
    : Widget(parent), m_state(NORMAL) {
    for (int s = 0; s < STATE_NO; s++) {
        setTexture(static_cast<State>(s), texture);
    }

    Widget::setTexture(texture);
}

void ButtonWidget::setOnClick(std::function<void()> func) {
    m_onClick = func;
}

void ButtonWidget::setTexture(State state, const std::string& texture) {
    m_stateTextures[state] =
        &Game::getInstance()->getTextureAtlas().getTexture(texture);
}

void ButtonWidget::handleEvent(sf::Event e) {
    if (e.type == sf::Event::MouseMoved) {
        sf::RenderWindow& win = Game::getInstance()->getWindow();
        UI* ui                = getUI();
        sf::Vector2i mousePos(e.mouseMove.x, e.mouseMove.y);
        sf::Vector2f pos;
        if (ui != nullptr) {
            pos = win.mapPixelToCoords(mousePos, ui->getView());
        } else {
            pos = win.mapPixelToCoords(mousePos);
        }

        if (getSprite().getGlobalBounds().contains(pos)) {
            if (m_state != PRESSED) {
                m_state = HOVERED;
                getSprite().setTexture(*m_stateTextures[m_state]);
            }
        } else {
            m_state = NORMAL;
            getSprite().setTexture(*m_stateTextures[m_state]);
        }
    } else if (e.type == sf::Event::MouseButtonPressed) {
        if (e.mouseButton.button == sf::Mouse::Left) {
            sf::RenderWindow& win = Game::getInstance()->getWindow();
            UI* ui                = getUI();
            sf::Vector2i mousePos(e.mouseButton.x, e.mouseButton.y);
            sf::Vector2f pos;
            if (ui != nullptr) {
                pos = win.mapPixelToCoords(mousePos, ui->getView());
            } else {
                pos = win.mapPixelToCoords(mousePos);
            }

            if (getSprite().getGlobalBounds().contains(pos)) {
                m_state = PRESSED;
                getSprite().setTexture(*m_stateTextures[m_state]);
            }
        }
    } else if (e.type == sf::Event::MouseButtonReleased) {
        if (e.mouseButton.button == sf::Mouse::Left) {
            sf::RenderWindow& win = Game::getInstance()->getWindow();
            UI* ui                = getUI();
            sf::Vector2i mousePos(e.mouseButton.x, e.mouseButton.y);
            sf::Vector2f pos;
            if (ui != nullptr) {
                pos = win.mapPixelToCoords(mousePos, ui->getView());
            } else {
                pos = win.mapPixelToCoords(mousePos);
            }

            m_state = NORMAL;
            getSprite().setTexture(*m_stateTextures[m_state]);

            if (getSprite().getGlobalBounds().contains(pos)) {
                if (m_onClick) {
                    m_onClick();
                }
            }
        }
    }
}

void ButtonWidget::update() {}

}