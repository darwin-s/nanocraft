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

#include <UI/PlayerUI.hpp>

namespace nc {

PlayerUI::PlayerUI() : m_toolBar("toolbar.png") {
    addWidget(&m_toolBar);
}

void PlayerUI::update() {
    const float texAR =
        static_cast<float>(m_toolBar.getSprite().getTextureRect().width) /
        static_cast<float>(m_toolBar.getSprite().getTextureRect().height);

    const float height = (0.5f / texAR) * (16.0f / 9.0f);
    const float yPos   = 1.0f - height;
    m_toolBar.setSize(0.5f, height);
    m_toolBar.getSprite().setPosition(0.25f, yPos);
}

}