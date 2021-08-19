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

#ifndef NC_GAME_ITEM_HPP
#define NC_GAME_ITEM_HPP

#include <World/Tile.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <string>

namespace nc {

class Item {
public:
    explicit Item(const std::string& name);
    Item(const std::string& texture, const std::string& name);
    void setTexture(const std::string& texture);
    sf::Sprite& getSprite();
    const sf::Sprite& getSprite() const;
    virtual Tile* getPlaceableTile() const;
    void setName(const std::string& name);
    std::string getName() const;

private:
    sf::Sprite m_sprite;
    std::string m_name;
};

}

#endif // !NC_GAME_ITEM_HPP