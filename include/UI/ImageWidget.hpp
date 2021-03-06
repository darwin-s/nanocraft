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

#ifndef NC_UI_IMAGEWIDGET_HPP
#define NC_UI_IMAGEWIDGET_HPP

#include <UI/Widget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <string>

namespace nc {

class ImageWidget : public Widget {
public:
    explicit ImageWidget(Widget* parent = nullptr);
    explicit ImageWidget(const std::string& texture, Widget* parent = nullptr);

protected:
    void handleEvent(sf::Event e) override;
    void update() override;
};

}

#endif // !NC_UI_IMAGEWIDGET_HPP