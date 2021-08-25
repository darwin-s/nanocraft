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

#include <Components/AnimationComponent.hpp>
#include <General/Object.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace nc {

AnimationComponent::AnimationComponent(entt::handle objectHandle,
                                       sf::Vector2i frameSize)
    : m_objectHandle(objectHandle), m_frameTimestep(1.0f), m_accum(0.0f),
      m_currentFrame(0), m_animFrames(0), m_shouldRepeat(false),
      m_frameRect(0, 0, frameSize.x, frameSize.y) {
    if (m_objectHandle.valid()) {
        if (Object* o = m_objectHandle.try_get<Object>(); o != nullptr) {
            o->setTextureRect(m_frameRect);
        }
    }
}

void AnimationComponent::setFramerate(float fps) {
    m_frameTimestep = 1.0f / fps;
}

float AnimationComponent::getFramerate() const {
    return 1.0f / m_frameTimestep;
}

void AnimationComponent::setFrameSize(sf::Vector2i size) {
    m_frameRect.width  = size.x;
    m_frameRect.height = size.y;

    if (m_objectHandle.valid()) {
        if (Object* o = m_objectHandle.try_get<Object>(); o != nullptr) {
            o->setTextureRect(m_frameRect);
        }
    }
}

void AnimationComponent::update(const float dt) {
    if (m_objectHandle.valid()) {
        if (Object* o = m_objectHandle.try_get<Object>(); o != nullptr) {
            m_accum += dt;

            while (m_accum >= m_frameTimestep) {
                m_frameRect.left += m_frameRect.width;
                m_currentFrame++;

                if (m_currentFrame >= m_animFrames) {
                    if (!m_requestedAnim.empty()) {
                        const AnimationInfo& ai = m_animations[m_requestedAnim];
                        m_accum                 = 0.0f;
                        m_currentFrame          = 0;
                        m_animFrames            = ai.frames;
                        m_shouldRepeat          = ai.repeated;
                        m_frameRect =
                            sf::IntRect(ai.firstFrame.x, ai.firstFrame.y,
                                        m_frameRect.width, m_frameRect.height);
                        m_currentAnim = m_requestedAnim;
                        m_requestedAnim.clear();
                    } else if (m_shouldRepeat) {
                        const AnimationInfo& ai = m_animations[m_currentAnim];
                        m_accum                 = 0.0f;
                        m_currentFrame          = 0;
                        m_frameRect =
                            sf::IntRect(ai.firstFrame.x, ai.firstFrame.y,
                                        m_frameRect.width, m_frameRect.height);
                    } else {
                        m_accum        = 0.0f;
                        m_currentFrame = 0;
                        m_animFrames   = 0;
                        m_shouldRepeat = false;
                        m_frameRect    = sf::IntRect(0, 0, m_frameRect.width,
                                                  m_frameRect.height);
                    }
                }

                o->setTextureRect(m_frameRect);

                if (m_accum != 0.0f) {
                    m_accum -= m_frameTimestep;
                }
            }
        }
    }
}

void AnimationComponent::addAnimation(const std::string& name,
                                      sf::Vector2i firstFrame,
                                      unsigned int frameNo, bool repeated) {
    AnimationInfo ai;
    ai.firstFrame = firstFrame;
    ai.frames     = frameNo;
    ai.repeated   = repeated;

    m_animations[name] = ai;
}

void AnimationComponent::startAnimation(const std::string& name, bool force) {
    if (m_animations.find(name) != m_animations.end()) {
        if (force) {
            const AnimationInfo& ai = m_animations[name];
            m_accum                 = 0.0f;
            m_currentFrame          = 0;
            m_animFrames            = ai.frames;
            m_shouldRepeat          = ai.repeated;
            m_frameRect   = sf::IntRect(ai.firstFrame.x, ai.firstFrame.y,
                                      m_frameRect.width, m_frameRect.height);
            m_currentAnim = name;

            if (m_objectHandle.valid()) {
                if (Object* o = m_objectHandle.try_get<Object>();
                    o != nullptr) {
                    o->setTextureRect(m_frameRect);
                }
            }
        } else {
            m_requestedAnim = name;
        }
    }
}

std::string AnimationComponent::getCurrentAnimation() const {
    return m_currentAnim;
}

}