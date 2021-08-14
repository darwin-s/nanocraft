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

#include <Widget.hpp>

namespace nc {

Widget::Widget(Widget* parent)
    : m_parent(parent), m_focused(false), m_ui(nullptr) {}

Widget* Widget::getParent() {
    return m_parent;
}

bool Widget::getFocused() const {
    return m_focused;
}

void Widget::setFocused(bool focus) {
    m_focused = focus;
}

UI* Widget::getUI() {
    return m_ui;
}

}