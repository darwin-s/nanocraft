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

#include <General/Physics.hpp>
#include <Components/VelocityComponent.hpp>
#include <Components/CollisionBoxComponent.hpp>
#include <General/Object.hpp>
#include <World/Map.hpp>
#include <SFML/Graphics/View.hpp>
#include <limits>
#include <cmath>

namespace nc {

void Physics::simulate(entt::registry& reg, float dt, Map* map) {
    reg.view<VelocityComponent, Object>().each(
        [&](auto ent, auto& vel, auto& obj) {
            if (vel.velocity.x == 0.0f && vel.velocity.y == 0.0f) {
                return;
            }

            sf::Vector2f velocity = vel.velocity * dt;

            // Handle collisions
            CollisionBoxComponent* cb = reg.try_get<CollisionBoxComponent>(ent);
            if (cb != nullptr) {
                // Handle tile collisions
                handleWorldCollision(cb, velocity, map);

                cb->box.left += velocity.x;
                cb->box.top += velocity.y;
            }

            obj.move(velocity);
            sf::View** v = reg.try_get<sf::View*>(ent);
            if (v != nullptr) {
                (*v)->move(velocity);
            }

            // Find magnitude of movement vector
            const float mag = std::sqrtf(vel.velocity.x * vel.velocity.x +
                                         vel.velocity.y * vel.velocity.y);

            // Find deceleration vector
            if (mag != 0.0f) {
                const sf::Vector2f dec =
                    vel.velocity * (-1.0f / mag * VELOCITY_DECEL) * dt;
                const float decMag = std::sqrtf(dec.x * dec.x + dec.y * dec.y);

                // Decelerate
                if (decMag >= mag) {
                    vel.velocity.x = 0.0f;
                    vel.velocity.y = 0.0f;
                } else {
                    vel.velocity += dec;
                }
            }
        });
}

void Physics::handleWorldCollision(const CollisionBoxComponent* cb,
                                   sf::Vector2f& v, Map* map) {
    Chunk* c[]      = {nullptr, nullptr, nullptr, nullptr};
    sf::Vector2u cp = Map::getChunkPos(cb->box.left, cb->box.top);
    c[0]            = map->getChunk(cp);
    sf::Vector2i norm(0, 0);

    // Compute velocity normal
    if (v.x > 0.0f) {
        norm.x = 1;
    } else if (v.x < 0.0f) {
        norm.x = -1;
    }

    if (v.y > 0.0f) {
        norm.y = 1;
    } else if (v.y < 0.0f) {
        norm.y = -1;
    }

    // Get chunks in direction
    if (norm.x != 0) {
        c[1] = map->getChunk(cp.x + norm.x, cp.y);
    }

    if (norm.y != 0) {
        c[2] = map->getChunk(cp.x, cp.y + norm.y);
    }

    if (norm.x != 0 && norm.y != 0) {
        c[3] = map->getChunk(cp.x + norm.x, cp.y + norm.y);
    }

    for (unsigned int i = 0; i < 4; i++) {
        if (c[i] != nullptr) {
            handleChunkCollision(cb, v, c[i]);
        }
    }
}

void Physics::handleChunkCollision(const CollisionBoxComponent* cb,
                                   sf::Vector2f& v, Chunk* c) {
    for (unsigned int y = 0; y < Chunk::CHUNK_SIZE; y++) {
        for (unsigned int x = 0; x < Chunk::CHUNK_SIZE; x++) {
            if (c->getTile(x, y).isCollidable()) {
                sf::FloatRect bp = getBroadphaseRect(cb->box, v);
                if (bp.intersects(c->getTile(x, y).getCollisionBox())) {
                    sf::Vector2f norm;
                    float collisionTime = getCollisionTime(
                        cb->box, c->getTile(x, y).getCollisionBox(), v, norm);
                    const float remainingTime = 1.0f - collisionTime;
                    if (collisionTime < 1.0f) {
                        float dotprod =
                            (v.x * norm.y + v.y * norm.x) * remainingTime;
                        v.x = dotprod * norm.y;
                        v.y = dotprod * norm.x;
                    }
                }
            }
        }
    }
}

float Physics::getCollisionTime(const sf::FloatRect& b1,
                                const sf::FloatRect& b2, const sf::Vector2f& v,
                                sf::Vector2f& normal) {
    float xEntryDist = 0.0f;
    float xExitDist  = 0.0f;
    float yEntryDist = 0.0f;
    float yExitDist  = 0.0f;

    float xEntry = 0.0f;
    float xExit  = 0.0f;
    float yEntry = 0.0f;
    float yExit  = 0.0f;

    if (v.x > 0.0f) {
        xEntryDist = b2.left - (b1.left + b1.width);
        xExitDist  = (b2.left + b2.width) - b1.left;
    } else {
        xEntryDist = (b2.left + b2.width) - b1.left;
        xExitDist  = b2.left - (b1.left + b1.width);
    }

    if (v.y > 0.0f) {
        yEntryDist = b2.top - (b1.top + b1.height);
        yExitDist  = (b2.top + b2.height) - b1.top;
    } else {
        yEntryDist = (b2.top + b2.height) - b1.top;
        yExitDist  = b2.top - (b1.top + b1.height);
    }

    if (v.x == 0.0f) {
        xEntry = -std::numeric_limits<float>::infinity();
        xExit  = std::numeric_limits<float>::infinity();
    } else {
        xEntry = xEntryDist / v.x;
        xExit  = xExitDist / v.x;
    }

    if (v.y == 0.0f) {
        yEntry = -std::numeric_limits<float>::infinity();
        yExit  = std::numeric_limits<float>::infinity();
    } else {
        yEntry = yEntryDist / v.y;
        yExit  = yExitDist / v.y;
    }

    float entryTime = std::max(xEntry, yEntry);
    float exitTime  = std::min(xExit, yExit);

    if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f ||
        xEntry > 1.0f || yEntry > 1.0f) {
        normal.x = 0.0f;
        normal.y = 0.0f;
        return 1.0f;
    } else {
        if (xEntry > yEntry) {
            if (xEntryDist < 0.0f) {
                normal.x = 1.0f;
                normal.y = 0.0f;
            } else {
                normal.x = -1.0f;
                normal.y = 0.0f;
            }
        } else {
            if (yEntryDist < 0.0f) {
                normal.x = 0.0f;
                normal.y = 1.0f;
            } else {
                normal.x = 0.0f;
                normal.y = -1.0f;
            }
        }
    }

    return entryTime;
}

sf::FloatRect Physics::getBroadphaseRect(const sf::FloatRect& r,
                                         const sf::Vector2f& v) {
    sf::FloatRect ret;
    ret.left   = v.x > 0.0f ? r.left : r.left + v.x;
    ret.top    = v.y > 0.0f ? r.top : r.top + v.y;
    ret.width  = v.x > 0.0f ? r.width + v.x : r.width - v.x;
    ret.height = v.y > 0.0f ? r.height + v.y : r.height - v.y;

    return ret;
}

}