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
#include <cassert>

namespace {

unsigned int PAGE_SIZE = 32;

}

namespace nc {

TextureAtlas::TextureAtlas(unsigned int tileSize) : m_tileSize(tileSize) {
    if (PAGE_SIZE == 0) {
        PAGE_SIZE = sf::Texture::getMaximumSize();
    }

    AtlasPage& p1 = m_textures[0];

    p1.xFreeOffset = 0;
    p1.yFreeOffset = 0;
    p1.full        = false;
    p1.tex.create(PAGE_SIZE, PAGE_SIZE);
}

bool TextureAtlas::addTexture(const std::filesystem::path& path) {
    bool couldLoad          = true;
    PHYSFS_File* fileHandle = PHYSFS_openRead(path.string().c_str());
    if (fileHandle == NULL) {
        // TODO log physfs error
        couldLoad = false;
    }
    PHYSFS_sint64 fileSize = PHYSFS_fileLength(fileHandle);
    if (fileSize == -1 && couldLoad) {
        // TODO log physfs error
        couldLoad = false;
    }
    unsigned char* fileData = new unsigned char[fileSize];
    PHYSFS_sint64 read      = PHYSFS_readBytes(fileHandle, fileData, fileSize);
    if (read < fileSize && couldLoad) {
        // TODO log physfs error
        couldLoad = false;
    }
    PHYSFS_close(fileHandle);

    sf::Image img;
    if (couldLoad) {
        img.loadFromMemory(fileData, fileSize);
    } else {
        img.create(m_tileSize, m_tileSize, sf::Color::Magenta);
    }

    delete[] fileData;

    // Check if texture has the right size
    if (img.getSize().x != m_tileSize || img.getSize().y != m_tileSize) {
        // TODO log warning
        img.create(m_tileSize, m_tileSize, sf::Color::Magenta);
        couldLoad = false;
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
    assert(m_texInfo.find(texture) != m_texInfo.end());
    return m_texInfo.at(texture);
}

}