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

#ifndef NC_UI_PLAYERUI_HPP
#define NC_UI_PLAYERUI_HPP

#include <UI/UI.hpp>
#include <UI/ImageWidget.hpp>
#include <Game/ItemStack.hpp>
#include <entt/entt.hpp>

namespace nc {

class PlayerUI : public UI {
public:
    PlayerUI();
    void update() override;
    void setPlayer(entt::const_handle m_player);
    void selectHotbarItem(unsigned int pos);
    unsigned int getSelectedHotbarItem() const;

private:
    ImageWidget m_toolBar;
    ImageWidget m_toolBarImages[9];
    ImageWidget m_selectedOverlay;
    unsigned int m_selectedPos;
    entt::const_handle m_player;
};

}

#endif // !NC_UI_PLAYERUI_HPP