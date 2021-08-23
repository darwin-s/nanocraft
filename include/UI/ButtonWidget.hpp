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

#ifndef NC_UI_BUTTONWIDGET_HPP
#define NC_UI_BUTTONWIDGET_HPP

#include <UI/Widget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <functional>
#include <array>

namespace nc {

class ButtonWidget : public Widget {
public:
    enum State { NORMAL = 0, HOVERED, PRESSED, STATE_NO };

public:
    explicit ButtonWidget(Widget* parent = nullptr);
    explicit ButtonWidget(const std::string& texture, Widget* parent = nullptr);
    void setOnClick(std::function<void()> func);
    void setTexture(State state, const std::string& texture);

protected:
    void handleEvent(sf::Event e) override;
    void update() override;

private:
    std::function<void()> m_onClick;
    State m_state;
    std::array<const sf::Texture*, STATE_NO> m_stateTextures;
};

}

#endif // !NC_UI_BUTTONWIDGET_HPP