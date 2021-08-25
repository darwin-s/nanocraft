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

#ifndef NC_COMPONENTS_ANIMATIONCOMPONENT_HPP
#define NC_COMPONENTS_ANIMATIONCOMPONENT_HPP

#include <SFML/Graphics/Rect.hpp>
#include <entt/entt.hpp>
#include <string>
#include <unordered_map>

namespace nc {

class AnimationComponent {
public:
    AnimationComponent(entt::handle objectHandle,
                       sf::Vector2i frameSize = sf::Vector2i(16, 32));
    void setFramerate(float fps);
    float getFramerate() const;
    void setFrameSize(sf::Vector2i size);
    void update(float dt);
    void addAnimation(const std::string& name, sf::Vector2i firstFrame,
                      unsigned int frameNo, bool repeated);
    void startAnimation(const std::string& name, bool force = false);
    std::string getCurrentAnimation() const;

private:
    struct AnimationInfo {
        sf::Vector2i firstFrame;
        unsigned int frames;
        bool repeated;
    };

private:
    entt::handle m_objectHandle;
    float m_frameTimestep;
    float m_accum;
    unsigned int m_currentFrame;
    unsigned int m_animFrames;
    bool m_shouldRepeat;
    sf::IntRect m_frameRect;
    std::unordered_map<std::string, AnimationInfo> m_animations;
    std::string m_currentAnim;
    std::string m_requestedAnim;
};

}

#endif // !NC_COMPONENTS_ANIMATIONCOMPONENT_HPP