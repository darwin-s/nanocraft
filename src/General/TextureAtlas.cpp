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

#include <General/TextureAtlas.hpp>
#include <physfs.h>
#include <spdlog/spdlog.h>

namespace nc {

TextureAtlas::TextureAtlas() {
    // Create default texture
    sf::Image img;
    img.create(16, 16, sf::Color::Magenta);
    sf::Texture t;
    t.loadFromImage(img);

    m_textures["default"] = std::move(t);
}

bool TextureAtlas::addTexture(const std::filesystem::path& path) {
    bool couldLoad          = true;
    PHYSFS_File* fileHandle = PHYSFS_openRead(path.string().c_str());
    PHYSFS_sint64 fileSize;
    PHYSFS_sint64 read;
    unsigned char* fileData = nullptr;
    if (fileHandle == NULL) {
        spdlog::warn("Could not open texture {}!", path.string());
        spdlog::warn("Using default texture!");
        couldLoad = false;
    } else {
        fileSize = PHYSFS_fileLength(fileHandle);
        if (fileSize == -1) {
            spdlog::warn("Could not retreive file size for texture {}!",
                         path.string());
            spdlog::warn("Using default texture!");
            couldLoad = false;
        }
        fileData = new unsigned char[fileSize];
        read     = PHYSFS_readBytes(fileHandle, fileData, fileSize);
        if (read < fileSize && couldLoad) {
            spdlog::warn("Could not read texture {}!", path.string());
            spdlog::warn("Using default texture!");
            couldLoad = false;
        }
        PHYSFS_close(fileHandle);
    }

    sf::Image img;
    if (couldLoad) {
        img.loadFromMemory(fileData, fileSize);
    } else {
        img.create(16, 16, sf::Color::Magenta);
    }

    delete[] fileData;

    sf::Texture t;
    t.loadFromImage(img);

    m_textures[path.string()] = std::move(t);

    return couldLoad;
}

const sf::Texture& TextureAtlas::getTexture(const std::string& texture) const {
    std::string actualPath;
    if (texture != "default") {
        actualPath = "/textures/" + texture;
    } else {
        actualPath = texture;
    }

    if (m_textures.find(actualPath) == m_textures.end()) {
        spdlog::warn("Could not find texture {}! Using default one!",
                     actualPath);
        return m_textures.at("default");
    }

    return m_textures.at(actualPath);
}

}