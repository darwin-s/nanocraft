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

#ifndef NC_GAMEREGISTRY_HPP
#define NC_GAMEREGISTRY_HPP

#include <unordered_map>
#include <string>

namespace nc {

class Item;
class Tile;

class GameRegistry {
public:
    GameRegistry();
    ~GameRegistry();
    void registerItem(Item* item);
    void registerTile(Tile* tile);
    Item* getItem(const std::string& name);
    Tile* getTile(const std::string& name);

private:
    std::unordered_map<std::string, Item*> m_items;
    std::unordered_map<std::string, Tile*> m_tiles;
};

}

#endif // !NC_GAMEREGISTRY_HPP