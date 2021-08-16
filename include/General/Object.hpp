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

#ifndef NC_GENERAL_OBJECT_HPP
#define NC_GENERAL_OBJECT_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

namespace nc {

class Object : public sf::Sprite {
public:
    explicit Object(sf::Vector2u size = sf::Vector2u(1, 1));
    explicit Object(const std::string& texture, sf::Vector2u size = sf::Vector2u(1, 1));
    void setTexture(const std::string& texture);
    sf::Vector2u getSize() const;
    void setSize(sf::Vector2u size);

private:
    sf::Vector2u m_size;
};

}

#endif // !NC_GENERAL_OBJECT_HPP
