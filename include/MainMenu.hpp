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

#ifndef NC_MAINMENU_HPP
#define NC_MAINMENU_HPP

#include <UI.hpp>
#include <ImageWidget.hpp>
#include <ButtonWidget.hpp>

namespace nc {

class MainMenu : public UI {
public:
    MainMenu();

private:
    ImageWidget m_background;
    ButtonWidget m_startGame;
};

}

#endif // !NC_MAINMENU_HPP