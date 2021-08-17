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

#ifndef NC_UI_WIDGET_HPP
#define NC_UI_WIDGET_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

namespace nc {

class UI;

class Widget : public sf::Drawable {
public:
    explicit Widget(Widget* parent = nullptr);
    Widget* getParent();
    bool getFocused() const;
    void setShown(bool show);
    bool getShown() const;
    sf::Sprite& getSprite();
    void setSize(float width, float height);
    void setSize(sf::Vector2f size);
    void setTexture(const std::string& texture);

protected:
    virtual void handleEvent(sf::Event e) = 0;
    virtual void update() = 0;
    void setFocused(bool focus);
    UI* getUI();

    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates states) const override;

private:
    friend class UI;
    Widget* m_parent;
    bool m_focused;
    bool m_shown;
    UI* m_ui;
    sf::Sprite m_sprite;
};

}

#endif // !NC_UI_WIDGET_HPP