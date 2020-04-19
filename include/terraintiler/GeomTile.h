#pragma once

#include "terraintiler/Renderable.h"

#include <SM_Rect.h>
#include <unirender2/typedef.h>

#include <array>
#include <vector>

namespace ur2 { class Device; }

namespace terraintiler
{

class GeomTile
{
public:
    GeomTile();

    Renderable GetRenderable(size_t lod) const;

    void Build(const ur2::Device& dev, const sm::rect& region,
        size_t width, size_t height, const std::vector<int32_t>& heights);

    bool IsBuilt() const { return m_built; }

public:
    ur2::TexturePtr heightmap = nullptr;

private:
    bool m_built = false;

    static const size_t MAX_LOD_LEVEL = 6;
    std::array<Renderable, MAX_LOD_LEVEL> m_rd;

}; // GeomTile

}