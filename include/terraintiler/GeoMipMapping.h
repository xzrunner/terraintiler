#pragma once

#include <unirender/Texture.h>

#include <vector>
#include <memory>

namespace terraintiler
{

class Tile;

class GeoMipMapping
{
public:
    GeoMipMapping(size_t width, size_t height);

    ur::TexturePtr QueryHeightmap(size_t x, size_t y) const;

    size_t GetWidth() const { return m_width; }
    size_t GetHeight() const { return m_height; }

private:
    struct Tile
    {
        ur::TexturePtr heightmap = nullptr;
    };

private:
    size_t m_width  = 0;
    size_t m_height = 0;

    mutable std::vector<Tile> m_tiles;

}; // GeoMipMapping

}