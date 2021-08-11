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

#include <Physics.hpp>
#include <VelocityComponent.hpp>
#include <Object.hpp>
#include <cmath>

namespace nc {

void Physics::simulate(entt::registry& reg, const float dt) {
    reg.view<VelocityComponent, Object>().each([=](auto& vel, auto& obj) {
        if (vel.velocity.x == 0.0f && vel.velocity.y == 0.0f) {
            return;
        }

        obj.move(vel.velocity * dt);

        // Find magnitude of movement vector
        const float mag = std::sqrtf(vel.velocity.x * vel.velocity.x +
                                     vel.velocity.y * vel.velocity.y);

        // Find deceleration vector
        const sf::Vector2f dec = vel.velocity * (-1.0f / mag * VELOCITY_DECEL) * dt;
        const float decMag     = std::sqrtf(dec.x * dec.x + dec.y * dec.y);

        // Decelerate
        if (decMag >= mag) {
            vel.velocity.x = 0.0f;
            vel.velocity.y = 0.0f;
        } else {
            vel.velocity += dec;
        }
    });
}

}