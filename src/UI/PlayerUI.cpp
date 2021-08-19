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
#include <Components/InventoryComponent.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace nc {

PlayerUI::PlayerUI() : m_toolBar("toolbar.png") {
    const float screenMiddleX = UI::REFERENCE_WIDTH / 2.0f;
    const float halfWidth     = m_toolBar.getSize().x / 2.0f;
    const float height        = m_toolBar.getSize().y;

    m_toolBar.setPosition(screenMiddleX - halfWidth,
                          UI::REFERENCE_HEIGHT - height);
    addWidget(&m_toolBar);

    for (unsigned int i = 0; i < 10; i++) {
        m_toolBarImages[i].setParent(&m_toolBar);
        m_toolBarImages[i].setPosition((float)i * 16.0f + 4, 4.0f);
        m_toolBarImages[i].setShown(false);
        addWidget(&m_toolBarImages[i]);
    }
}

void PlayerUI::update() {
    if (m_player.valid()) {
        const InventoryComponent& inv = m_player.get<InventoryComponent>();

        for (int i = 0; i < 10; i++) {
            Item* itm = inv.inventory[i].getItem();
            if (itm != nullptr) {
                const sf::Texture* itemTex = itm->getSprite().getTexture();

                if (itemTex != nullptr) {
                    m_toolBarImages[i].getSprite().setTexture(*itemTex);
                    m_toolBarImages[i].getSprite().setTextureRect(
                        itm->getSprite().getTextureRect());
                    m_toolBarImages[i].setShown(true);
                } else {
                    m_toolBarImages[i].setShown(false);
                }
            } else {
                m_toolBarImages[i].setShown(false);
            }
        }
    }
}

void PlayerUI::setPlayer(entt::const_handle player) {
    m_player = player;
}

}