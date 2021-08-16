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

#include <TextureAtlas.hpp>
#include <physfs.h>
#include <spdlog/spdlog.h>

namespace nc {

TextureAtlas::TextureAtlas(unsigned int tileSize)
    : m_tileSize(tileSize), m_freeSprite(0) {
    AtlasPage& p1 = m_textures[0];

    p1.xFreeOffset = m_tileSize;
    p1.yFreeOffset = 0;
    p1.full        = false;
    p1.tex.create(PAGE_SIZE, PAGE_SIZE);

    // Create default texture
    sf::Image img;
    img.create(m_tileSize, m_tileSize, sf::Color::Magenta);
    p1.tex.update(img, 0, 0);

    // Update the map
    TextureInfo ti;
    ti.texture            = &m_textures[0].tex;
    ti.textureRect.left   = 0;
    ti.textureRect.top    = 0;
    ti.textureRect.width  = m_tileSize;
    ti.textureRect.height = m_tileSize;
    ti.textureIndex       = 0;

    m_texInfo["default"] = ti;
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
        img.create(m_tileSize, m_tileSize, sf::Color::Magenta);
    }

    delete[] fileData;

    // Check if texture has the right size
    if (img.getSize().x != m_tileSize || img.getSize().y != m_tileSize) {
        sf::Texture t;
        t.loadFromImage(img);
        m_otherTextures[m_freeSprite++] = std::move(t);

        TextureInfo ti;
        ti.texture            = &m_otherTextures[m_freeSprite - 1];
        ti.textureRect.left   = 0;
        ti.textureRect.top    = 0;
        ti.textureRect.width  = img.getSize().x;
        ti.textureRect.height = img.getSize().y;
        ti.textureIndex       = 0;

        m_texInfo[path.string()] = ti;

        return true;
    }

    // Find first free page
    unsigned int freeIndex = MAX_TEXTURES + 1;
    for (unsigned int i = 0; i < MAX_TEXTURES; i++) {
        if (!m_textures[i].full) {
            freeIndex = i;
            break;
        }
    }
    // Error if no free space remains
    if (freeIndex == MAX_TEXTURES + 1) {
        throw std::runtime_error("No free space in texture atlas left!");
    }

    // Get the free space
    if (m_textures[freeIndex].xFreeOffset == PAGE_SIZE) {
        m_textures[freeIndex].xFreeOffset = 0;
        m_textures[freeIndex].yFreeOffset += m_tileSize;
    }

    // Update part of the texture
    m_textures[freeIndex].tex.update(img, m_textures[freeIndex].xFreeOffset,
                                     m_textures[freeIndex].yFreeOffset);

    // Update the map
    TextureInfo ti;
    ti.texture            = &m_textures[freeIndex].tex;
    ti.textureRect.left   = m_textures[freeIndex].xFreeOffset;
    ti.textureRect.top    = m_textures[freeIndex].yFreeOffset;
    ti.textureRect.width  = m_tileSize;
    ti.textureRect.height = m_tileSize;
    ti.textureIndex       = freeIndex;

    m_texInfo[path.string()] = ti;

    // Set next free part
    m_textures[freeIndex].xFreeOffset += m_tileSize;

    // Check if this was the last space
    if (m_textures[freeIndex].xFreeOffset == PAGE_SIZE &&
        m_textures[freeIndex].yFreeOffset == PAGE_SIZE - m_tileSize) {
        m_textures[freeIndex].full = true;
        // Allocate next page if possible
        if (freeIndex < MAX_TEXTURES - 1) {
            AtlasPage& p = m_textures[freeIndex + 1];

            p.xFreeOffset = 0;
            p.yFreeOffset = 0;
            p.full        = false;
            p.tex.create(PAGE_SIZE, PAGE_SIZE);
        }
    }

    return couldLoad;
}

TextureAtlas::TextureInfo
    TextureAtlas::getTexture(const std::string& texture) const {
    if (m_texInfo.find(texture) == m_texInfo.end()) {
        spdlog::warn("Could not find texture {}! Using default one!", texture);
        return m_texInfo.at("default");
    }

    return m_texInfo.at(texture);
}

unsigned int TextureAtlas::getTileSize() const {
    return m_tileSize;
}

}