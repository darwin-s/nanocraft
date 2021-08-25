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

#include <General/InputHandler.hpp>
#include <Game/Game.hpp>
#include <Components/PlayerInputComponent.hpp>
#include <Components/VelocityComponent.hpp>
#include <Components/AnimationComponent.hpp>

namespace {
    static constexpr float PLAYER_VEL = 2.5f;
    static constexpr float SHIFT_FACTOR = 5.0f;
    static constexpr float CTRL_FACTOR = 0.2f;

    sf::Keyboard::Key getKey(const char* key, const nlohmann::json& sett) {
        return sett["controls"][key].get<sf::Keyboard::Key>();
    }
}

namespace nc {

void InputHandler::handleInput(const sf::Event& e, entt::registry& reg) {
    
}

void InputHandler::pollInput(entt::registry& reg) {
    const nlohmann::json& sett = Game::getInstance()->getSettings();

    float factor = 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
        factor = SHIFT_FACTOR;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
        factor = CTRL_FACTOR;
    }

    reg.view<PlayerInputComponent, VelocityComponent>().each([&](auto p, auto& vel) {
        AnimationComponent* ac = reg.try_get<AnimationComponent>(p);
        bool movingUp = false;
        bool movingDown = false;
        bool movingLeft = false;
        bool movingRight = false;
        std::string currentAnim;

        if (sf::Keyboard::isKeyPressed(getKey("move_up", sett))) {
            vel.velocity.y = -PLAYER_VEL * factor;
            movingUp = true;
            movingDown = false;
        }

        if (sf::Keyboard::isKeyPressed(getKey("move_down", sett))) {
            vel.velocity.y = PLAYER_VEL * factor;
            movingUp = false;
            movingDown = true;
        }

        if (sf::Keyboard::isKeyPressed(getKey("move_left", sett))) {
            vel.velocity.x = -PLAYER_VEL * factor;
            movingLeft = true;
            movingRight = false;
        }

        if (sf::Keyboard::isKeyPressed(getKey("move_right", sett))) {
            vel.velocity.x = PLAYER_VEL * factor;
            movingLeft = false;
            movingRight = true;
        }

        if (ac == nullptr) {
            return;
        }

        currentAnim = ac->getCurrentAnimation();

        if (movingLeft && !movingUp && !movingDown) {
            if (currentAnim != "walk_left") {
                ac->startAnimation("walk_left", true);
            }
        } else if (movingRight && !movingUp && !movingDown) {
            if (currentAnim != "walk_right") {
                ac->startAnimation("walk_right", true);
            }
        }

        if (movingUp) {
            if (currentAnim != "walk_up") {
                ac->startAnimation("walk_up", true);
            }
        } else if (movingDown) {
            if (currentAnim != "walk_down") {
                ac->startAnimation("walk_down", true);
            }
        }

        if (!movingUp && !movingDown && !movingLeft && !movingRight) {
            if (currentAnim != "idle") {
                ac->startAnimation("idle", true);
            }
        }
    });
}

}