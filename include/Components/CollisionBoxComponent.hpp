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

#ifndef NC_COMPONENTS_COLLISIONBOXCOMPONENT_HPP
#define NC_COMPONENTS_COLLISIONBOXCOMPONENT_HPP

#include <SFML/Graphics/Rect.hpp>

namespace nc {

struct CollisionBoxComponent {
    sf::FloatRect box;
};
    
}

#endif // !NC_COMPONENTS_COLLISIONBOXCOMPONENT_HPP