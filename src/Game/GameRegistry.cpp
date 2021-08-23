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

#include <Game/GameRegistry.hpp>
#include <Game/Item.hpp>
#include <World/Tile.hpp>

namespace nc {

GameRegistry::GameRegistry() {}

GameRegistry::~GameRegistry() {
    for (auto& i : m_items) {
        delete i.second;
    }

    for (auto& t : m_tiles) {
        delete t.second;
    }
}

void GameRegistry::registerItem(Item* item) {
    m_items[item->getName()] = item;
}

void GameRegistry::registerTile(Tile* tile) {
    m_tiles[tile->getName()] = tile;
}

Item* GameRegistry::getItem(const std::string& name) {
    if (m_items.find(name) == m_items.end()) {
        return nullptr;
    }

    return m_items[name];
}

Tile* GameRegistry::getTile(const std::string& name) {
    if (m_tiles.find(name) == m_tiles.end()) {
        return nullptr;
    }

    return m_tiles[name];
}

}