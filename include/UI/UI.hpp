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

#ifndef NC_UI_UI_HPP
#define NC_UI_UI_HPP

#include <UI/Widget.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>

namespace nc {

class UI : public sf::Drawable {
public:
    UI();
    void setShown(bool shown);
    bool getShown() const;
    void handleEvent(sf::Event e);
    void addWidget(Widget* w);
    void setFocus(Widget* w);
    sf::View& getView();

protected:
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates states) const override;

private:
    bool m_shown;
    sf::View m_view;
    std::vector<Widget*> m_widgets;
    Widget* m_focused;
};

}

#endif // !NC_UI_UI_HPP