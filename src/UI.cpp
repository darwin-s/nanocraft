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

#include <UI.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace nc {

UI::UI()
    : m_shown(false), m_view(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f)),
      m_focused(nullptr) {}

void UI::setShown(bool shown) {
    m_shown = shown;
}

bool UI::getShown() const {
    return m_shown;
}

void UI::handleEvent(sf::Event e) {
    for (auto& w : m_widgets) {
        w->handleEvent(e);
    }
}

void UI::addWidget(Widget* w) {
    m_widgets.push_back(w);
    w->m_ui = this;
}

void UI::setFocus(Widget* w) {
    if (m_focused != nullptr) {
        m_focused->setFocused(false);
    }

    m_focused = w;
    m_focused->setFocused(true);
}

void UI::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (!m_shown) {
        return;
    }

    const sf::View v = target.getView();

    target.setView(m_view);
    for (const auto& w : m_widgets) {
        if (w->getShown() == true) {
            w->draw(target, states);
        }
    }

    target.setView(v);
}

}