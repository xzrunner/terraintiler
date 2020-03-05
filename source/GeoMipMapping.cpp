#include "terraintiler/GeoMipMapping.h"

#include <heightfield/HeightField.h>
#include <terraingraph/device/CellularNoise.h>

namespace terraintiler
{

GeoMipMapping::GeoMipMapping(size_t width, size_t height)
    : m_width(width)
    , m_height(height)
{
    m_tiles.resize(width * height);
}

ur::TexturePtr GeoMipMapping::QueryHeightmap(size_t x, size_t y) const
{
    if (x >= m_width || y >= m_height) {
        return nullptr;
    }

    auto& tile = m_tiles[y * m_width + x];
    if (!tile.heightmap)
    {
        terraingraph::device::CellularNoise noise;
        noise.SetResolution(sm::vec2(1, 1));
        noise.SetOffset(sm::vec2(x, y));

        noise.SetWidth(64);
        noise.SetHeight(64);

        noise.Execute();

        tile.heightmap = noise.GetHeightField()->GetHeightmap();
    }

    return tile.heightmap;
}

}