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

#ifndef NC_GENERAL_PHYSICS_HPP
#define NC_GENERAL_PHYSICS_HPP

#include <SFML/Graphics/Rect.hpp>
#include <entt/entt.hpp>

namespace nc {

class Chunk;
class Map;
struct CollisionBoxComponent;

class Physics {
public:
    static constexpr float VELOCITY_DECEL = 20.0f;

public:
    static void simulate(entt::registry& reg, float dt, Map* map);
    static void handleWorldCollision(const CollisionBoxComponent* cb,
                                     sf::Vector2f& v, Map* map);
    static void handleChunkCollision(const CollisionBoxComponent* cb,
                                     sf::Vector2f& v, Chunk* c);
    static float getCollisionTime(const sf::FloatRect& b1,
                                  const sf::FloatRect& b2,
                                  const sf::Vector2f& v, sf::Vector2f& normal);
    static sf::FloatRect getBroadphaseRect(const sf::FloatRect& r,
                                           const sf::Vector2f& v);
};

}

#endif // !NC_GENERAL_PHYSICS_HPP