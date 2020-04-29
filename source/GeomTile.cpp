#include "terraintiler/GeomTile.h"

#include <heightfield/Utility.h>
#include <unirender/Device.h>
#include <unirender/VertexArray.h>
#include <unirender/VertexBuffer.h>
#include <unirender/IndexBuffer.h>
#include <unirender/VertexBuffer.h>

#include <vector>

namespace terraintiler
{

GeomTile::GeomTile()
{
}

Renderable GeomTile::GetRenderable(size_t lod) const
{
    return lod >= MAX_LOD_LEVEL ? m_rd[MAX_LOD_LEVEL - 1] : m_rd[lod];
}

void GeomTile::Build(const ur::Device& dev, const sm::rect& region, size_t width,
                     size_t height, const std::vector<int32_t>& heights)
{
    m_built = true;

    assert(width * height == heights.size()
        && width > std::pow(2, MAX_LOD_LEVEL)
        && height > std::pow(2, MAX_LOD_LEVEL));

    std::vector<sm::vec3> verts;
    verts.resize(heights.size());
    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            const auto idx = y * width + x;
            auto& v = verts[idx];
            v.x = region.xmin + (region.xmax - region.xmin) / (width - 1) * x;
            v.y = hf::Utility::HeightShortToFloat(heights[idx]);
            v.z = region.ymin + (region.ymax - region.ymin) / (height - 1) * y;
        }
    }

    size_t w = width;
    size_t h = height;
    for (size_t i = 0; i < MAX_LOD_LEVEL; ++i)
    {
        std::vector<uint32_t> indices;
        indices.resize((w - 1) * (h - 1) * 6);
        assert(indices.size() < 0xffffffff);
        auto idx_ptr = indices.data();
        for (size_t y = 0; y < h - 1; ++y) {
            for (size_t x = 0; x < w - 1; ++x) {
                uint32_t ll = y * w + x;
                uint32_t rl = ll + 1;
                uint32_t lh = ll + w;
                uint32_t rh = lh + 1;
                *idx_ptr++ = ll;
                *idx_ptr++ = lh;
                *idx_ptr++ = rh;
                *idx_ptr++ = ll;
                *idx_ptr++ = rh;
                *idx_ptr++ = rl;
            }
        }

        auto va = dev.CreateVertexArray();

        auto ibuf_sz = sizeof(uint32_t) * indices.size();
        auto ibuf = dev.CreateIndexBuffer(ur::BufferUsageHint::StaticDraw, ibuf_sz);
        ibuf->ReadFromMemory(indices.data(), ibuf_sz, 0);
        va->SetIndexBuffer(ibuf);

        auto vbuf_sz = sizeof(sm::vec3) * verts.size();
        auto vbuf = dev.CreateVertexBuffer(ur::BufferUsageHint::StaticDraw, vbuf_sz);
        vbuf->ReadFromMemory(verts.data(), vbuf_sz, 0);
        va->SetVertexBuffer(vbuf);

        m_rd[i].va = va;

        if (i == MAX_LOD_LEVEL - 1) {
            break;
        }

        size_t new_w = w / 2 + 1;
        size_t new_h = h / 2 + 1;
        std::vector<sm::vec3> new_verts(new_w * new_h);
        for (size_t y = 0; y < new_h; ++y) {
            for (size_t x = 0; x < new_w; ++x) {
                new_verts[y * new_w + x] = verts[y * 2 * w + x * 2];
            }
        }

        w = new_w;
        h = new_h;
        verts = new_verts;
    }
}

}