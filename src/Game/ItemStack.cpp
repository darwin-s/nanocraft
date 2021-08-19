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

#include <Game/ItemStack.hpp>

namespace nc {

ItemStack::ItemStack() : m_item(nullptr), m_count(0) {}

void ItemStack::setItem(Item* item, unsigned int count) {
    if (count == 0) {
        m_item = nullptr;
    } else {
        m_item = item;
    }

    m_count = count;
}

Item* ItemStack::getItem() {
    return m_item;
}

const Item* ItemStack::getItem() const {
    return m_item;
}

unsigned int ItemStack::getCount() const {
    return m_count;
}

void ItemStack::setCount(unsigned int count) {
    m_count = count;

    if (count == 0) {
        m_item = nullptr;
    }
}

bool ItemStack::isEmpty() const {
    return m_item == nullptr;
}

}