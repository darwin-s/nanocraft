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

#ifndef NC_GAME_ITEMSTACK_HPP
#define NC_GAME_ITEMSTACK_HPP

#include <Game/Item.hpp>

namespace nc {

class ItemStack {
public:
    ItemStack();
    void setItem(Item* item, unsigned int count = 1);
    Item* getItem();
    const Item* getItem() const;
    unsigned int getCount() const;
    void setCount(unsigned int count);
    bool isEmpty() const;

private:
    Item* m_item;
    unsigned int m_count;
};

}

#endif // !NC_GAME_ITEMSTACK_HPP