 #include "terraintiler/GeoMipMapping.h"

#include <heightfield/HeightField.h>
#include <terraingraph/device/CellularNoise.h>

namespace
{

std::vector<std::shared_ptr<terraingraph::Device>> cached;

}

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
        noise.SetResolution(sm::vec2(1.0f, 1.0f));
        noise.SetOffset(sm::vec2(static_cast<float>(x), static_cast<float>(y)));

        noise.SetWidth(65);
        noise.SetHeight(65);

        noise.Execute();

//        tile.heightmap = noise.GetHeightField()->GetHeightmap();
    }

    return tile.heightmap;
}

Renderable GeoMipMapping::QueryRenderable(size_t x, size_t y, size_t level) const
{
    if (x >= m_width || y >= m_height) {
        return Renderable();
    }

    auto& tile = m_tiles[y * m_width + x];
    return tile.GetRenderable(level);
}

void GeoMipMapping::UpdateTile(const ur::Device& dev, size_t x, size_t y)
{
    if (x >= m_width || y >= m_height) {
        return;
    }

    auto& tile = m_tiles[y * m_width + x];
    if (!tile.IsBuilt())
    {
        auto noise = std::make_shared<terraingraph::device::CellularNoise>();
        cached.push_back(noise);

        noise->SetResolution(sm::vec2(1, 1));
        noise->SetOffset(sm::vec2(static_cast<float>(x), static_cast<float>(y)));

        noise->SetWidth(65);
        noise->SetHeight(65);
        //noise->SetWidth(1025);
        //noise->SetHeight(1025);

        noise->Execute();

        auto hf = noise->GetHeightField();

        sm::rect r;
        r.xmin = static_cast<float>(x);
        r.xmax = r.xmin + 1;
        r.ymin = static_cast<float>(y);
        r.ymax = r.ymin + 1;

        tile.Build(dev, r, hf->Width(), hf->Height(), hf->GetValues(dev));
    }
}

}