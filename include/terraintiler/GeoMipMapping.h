#pragma once

#include "terraintiler/GeomTile.h"

#include <unirender/Texture.h>

#include <vector>

namespace terraintiler
{

class GeoMipMapping
{
public:
    GeoMipMapping(size_t width, size_t height);

    ur::TexturePtr QueryHeightmap(size_t x, size_t y) const;

    Renderable QueryRenderable(size_t x, size_t y, size_t level) const;

    void UpdateTile(size_t x, size_t y);

    size_t GetWidth() const { return m_width; }
    size_t GetHeight() const { return m_height; }

private:
    size_t m_width  = 0;
    size_t m_height = 0;

    std::vector<GeomTile> m_tiles;

}; // GeoMipMapping

}