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

#include <UI/PlayerInventory.hpp>
#include <Components/InventoryComponent.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace nc {

PlayerInventory::PlayerInventory() : m_invScren("inventory.png") {
    const float screenMiddleX = UI::REFERENCE_WIDTH / 2.0f;
    const float screenMiddleY = UI::REFERENCE_HEIGHT / 2.0f;
    const float halfWidth     = m_invScren.getSize().x / 2.0f;
    const float halfHeight    = m_invScren.getSize().y / 2.0f;

    m_invScren.setPosition(screenMiddleX - halfWidth,
                           screenMiddleY - halfHeight);
    addWidget(&m_invScren);

    for (unsigned int y = 0; y < PLAYER_INV_ROWS; y++) {
        for (unsigned int x = 0; x < PLAYER_INV_COLS; x++) {
            m_itemImages[y * PLAYER_INV_COLS + x].setParent(&m_invScren);
            m_itemImages[y * PLAYER_INV_COLS + x].setPosition(
                static_cast<float>(x) * 24.0f + 4,
                static_cast<float>(y) * 24.0f + 4.0f);
            m_itemImages[y * PLAYER_INV_COLS + x].setShown(false);
            addWidget(&m_itemImages[y * PLAYER_INV_COLS + x]);
        }
    }
}

void PlayerInventory::update() {
    if (m_player.valid()) {
        const InventoryComponent& inv = m_player.get<InventoryComponent>();

        for (unsigned int y = 0; y < PLAYER_INV_ROWS; y++) {
            for (unsigned int x = 0; x < PLAYER_INV_COLS; x++) {
                Item* itm = inv.inventory[y * PLAYER_INV_COLS + x].getItem();
                if (itm != nullptr) {
                    const sf::Texture* itemTex = itm->getSprite().getTexture();

                    if (itemTex != nullptr) {
                        m_itemImages[y * PLAYER_INV_COLS + x]
                            .getSprite()
                            .setTexture(*itemTex);
                        m_itemImages[y * PLAYER_INV_COLS + x]
                            .getSprite()
                            .setTextureRect(itm->getSprite().getTextureRect());
                        m_itemImages[y * PLAYER_INV_COLS + x].setShown(true);
                    } else {
                        m_itemImages[y * PLAYER_INV_COLS + x].setShown(false);
                    }
                } else {
                    m_itemImages[y * PLAYER_INV_COLS + x].setShown(false);
                }
            }
        }
    }
}

void PlayerInventory::setPlayer(entt::handle player) {
    m_player = player;
}

}