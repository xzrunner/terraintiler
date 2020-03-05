#pragma once

#include "terraintiler/Renderable.h"

#include <SM_Rect.h>

#include <array>
#include <vector>

namespace ur { class Texture; }

namespace terraintiler
{

class GeomTile
{
public:
    GeomTile();

    Renderable GetRenderable(size_t lod) const;

    void Build(const sm::rect& region, size_t width, size_t height,
        const std::vector<float>& heights);

    bool IsBuilt() const { return m_built; }

public:
    std::shared_ptr<ur::Texture> heightmap = nullptr;

private:
    bool m_built = false;

    static const size_t MAX_LOD_LEVEL = 6;
    std::array<Renderable, MAX_LOD_LEVEL> m_rd;

}; // GeomTile

}