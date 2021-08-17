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

#include <Game/PlayingState.hpp>
#include <Game/Game.hpp>
#include <General/Object.hpp>
#include <General/InputHandler.hpp>
#include <Components/PlayerInputComponent.hpp>
#include <Components/PlayerComponent.hpp>
#include <Components/VelocityComponent.hpp>
#include <General/Physics.hpp>

namespace nc {

PlayingState::PlayingState()
    : m_gen(new OverworldGenerator(Game::getInstance()
                                       ->getSettings()["debug"]["test_seed"]
                                       .get<unsigned int>())),
      m_map(new Map(m_gen)) {
    entt::registry& reg = m_map->getRegistry();
    m_player            = reg.create();
    reg.emplace<Object>(m_player, "player.png");
    reg.emplace<PlayerComponent>(m_player);
    reg.emplace<PlayerInputComponent>(m_player);
    reg.emplace<VelocityComponent>(m_player);
    reg.emplace<sf::View*>(m_player, &Game::getInstance()->getView());
    reg.get<Object>(m_player).setSize(sf::Vector2u(1, 2));
    reg.get<Object>(m_player).setPosition(sf::Vector2f(16400.0f, 16400.0f));
    reg.get<sf::View*>(m_player)->setCenter(16400.0f, 16400.0f);
    m_playerUI.setAspectRatio(16.0f / 9.0f);
    m_playerUI.setShown(true);
}

PlayingState::~PlayingState() {
    delete m_map;
    delete m_gen;
}

void PlayingState::perFrame() {
    InputHandler::pollInput(m_map->getRegistry());
    m_playerUI.update();
}

void PlayingState::handleEvent(sf::Event e) {
    InputHandler::handleInput(e, m_map->getRegistry());
    m_playerUI.handleEvent(e);
}

void PlayingState::update(const float dt) {
    // Simulate physics
    Physics::simulate(m_map->getRegistry(), dt);
    // Simulate world
    m_map->simulateWorld(dt);
}

void PlayingState::draw(sf::RenderWindow& win) {
    unsigned int chunkX =
        Map::getChunkPos(
            m_map->getRegistry().get<Object>(m_player).getPosition())
            .x;
    unsigned int chunkY =
        Map::getChunkPos(
            m_map->getRegistry().get<Object>(m_player).getPosition())
            .y;
    Chunk* c0 = m_map->getChunk(chunkX, chunkY);
    Chunk* c1 = m_map->getChunk(chunkX - 1, chunkY - 1);
    Chunk* c2 = m_map->getChunk(chunkX, chunkY - 1);
    Chunk* c3 = m_map->getChunk(chunkX + 1, chunkY - 1);
    Chunk* c4 = m_map->getChunk(chunkX - 1, chunkY);
    Chunk* c5 = m_map->getChunk(chunkX + 1, chunkY);
    Chunk* c6 = m_map->getChunk(chunkX - 1, chunkY + 1);
    Chunk* c7 = m_map->getChunk(chunkX, chunkY + 1);
    Chunk* c8 = m_map->getChunk(chunkX + 1, chunkY + 1);

    if (c0 != nullptr) {
        win.draw(*c0);
    }
    if (c1 != nullptr) {
        win.draw(*c1);
    }
    if (c2 != nullptr) {
        win.draw(*c2);
    }
    if (c3 != nullptr) {
        win.draw(*c3);
    }
    if (c4 != nullptr) {
        win.draw(*c4);
    }
    if (c5 != nullptr) {
        win.draw(*c5);
    }
    if (c6 != nullptr) {
        win.draw(*c6);
    }
    if (c7 != nullptr) {
        win.draw(*c7);
    }
    if (c8 != nullptr) {
        win.draw(*c8);
    }
    win.draw(m_map->getRegistry().get<Object>(m_player));
    // Draw ui
    win.draw(m_playerUI);
}

}