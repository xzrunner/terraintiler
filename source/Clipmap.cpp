#include "terraintiler/Clipmap.h"

#include <SM_Vector.h>
#include <unirender/Blackboard.h>
#include <unirender/RenderContext.h>
#include <heightfield/HeightField.h>
#include <terraingraph/device/CellularNoise.h>

#include <vector>

namespace
{

const size_t RESOLUTION = 1024;
//const size_t RESOLUTION = 128;
const float SCALE = 2.0f;
const float START_SZ = 1.0f;

}

namespace terraintiler
{

Clipmap::Clipmap()
{
    Build();

    // todo

    terraingraph::device::CellularNoise noise;
    noise.SetWidth(RESOLUTION);
    noise.SetHeight(RESOLUTION);
    noise.SetOffset(sm::vec2(-0.5f, -0.5f));

    sm::vec2 res(1.0f, 1.0f);
    for (size_t i = 0; i < LAYER_NUM; ++i)
    {
        noise.SetResolution(res);
        res /= 2;

        noise.Execute();

        m_layers[i].heightmap = noise.GetHeightField()->GetHeightmap();
    }
}

void Clipmap::Build()
{
    const float S_BLOCK = 1.0f / 4;
    const size_t RES = static_cast<size_t>(RESOLUTION * S_BLOCK);

    sm::rect layer_region(-0.5f, -0.5f, 0.5f, 0.5f);

    // layer0
    m_layers[0].blocks.resize(16);
    size_t ptr = 0;
    const float block_sz = START_SZ * S_BLOCK;
    for (size_t y = 0; y < 4; ++y)
    {
        for (size_t x = 0; x < 4; ++x)
        {
            sm::rect r;
            r.xmin = x * block_sz - START_SZ * 0.5f;
            r.xmax = r.xmin + block_sz;
            r.ymin = y * block_sz - START_SZ * 0.5f;
            r.ymax = r.ymin + block_sz;
            m_layers[0].blocks[ptr] = BuildBlock(r, RES, 1);

            ++ptr;
        }
    }

    // other layers
    for (size_t i = 1; i < LAYER_NUM; ++i)
    {
        auto& blocks = m_layers[i].blocks;
        blocks.resize(12);

        const float scale = static_cast<float>(std::pow(SCALE, i));
        const float sz = START_SZ * scale;
        const float h_sz = sz * 0.5f;
        const float hh_sz = h_sz * 0.5f;

        blocks[0] = BuildBlock(sm::rect( -h_sz, hh_sz, -hh_sz, h_sz), RES, scale);
        blocks[1] = BuildBlock(sm::rect(-hh_sz, hh_sz,      0, h_sz), RES, scale);
        blocks[2] = BuildBlock(sm::rect(     0, hh_sz,  hh_sz, h_sz), RES, scale);
        blocks[3] = BuildBlock(sm::rect( hh_sz, hh_sz,   h_sz, h_sz), RES, scale);

        blocks[4] = BuildBlock(sm::rect( -h_sz,      0, -hh_sz, hh_sz), RES, scale);
        blocks[5] = BuildBlock(sm::rect( hh_sz,      0,   h_sz, hh_sz), RES, scale);
        blocks[6] = BuildBlock(sm::rect( -h_sz, -hh_sz, -hh_sz,     0), RES, scale);
        blocks[7] = BuildBlock(sm::rect( hh_sz, -hh_sz,   h_sz,     0), RES, scale);

        blocks[8]  = BuildBlock(sm::rect( -h_sz, -h_sz, -hh_sz, -hh_sz), RES, scale);
        blocks[9]  = BuildBlock(sm::rect(-hh_sz, -h_sz,      0, -hh_sz), RES, scale);
        blocks[10] = BuildBlock(sm::rect(     0, -h_sz,  hh_sz, -hh_sz), RES, scale);
        blocks[11] = BuildBlock(sm::rect( hh_sz, -h_sz,   h_sz, -hh_sz), RES, scale);
    }
}

Clipmap::Block Clipmap::BuildBlock(const sm::rect& region, size_t res, float scale)
{
    std::vector<sm::vec2> verts(res * res);
    for (size_t y = 0; y < res; ++y)
    {
        for (size_t x = 0; x < res; ++x)
        {
            const auto idx = y * res + x;
            auto& v = verts[idx];
            v.x = region.xmin + (region.xmax - region.xmin) / (res - 1) * x;
            v.y = region.ymin + (region.ymax - region.ymin) / (res - 1) * y;
        }
    }

    Renderable rd;

    auto& rc = ur::Blackboard::Instance()->GetRenderContext();

    rd.vbo = rc.CreateBuffer(ur::VERTEXBUFFER, verts.data(), sizeof(sm::vec2) * verts.size());

    std::vector<uint16_t> indices;
    indices.resize((res - 1) * (res - 1) * 6);
    assert(indices.size() < 0xffffffff);
    auto idx_ptr = indices.data();
    for (size_t y = 0; y < res - 1; ++y) {
        for (size_t x = 0; x < res - 1; ++x) {
            uint16_t ll = static_cast<uint16_t>(y * res + x);
            uint16_t rl = ll + 1;
            uint16_t lh = static_cast<uint16_t>(ll + res);
            uint16_t rh = lh + 1;
            *idx_ptr++ = ll;
            *idx_ptr++ = lh;
            *idx_ptr++ = rh;
            *idx_ptr++ = ll;
            *idx_ptr++ = rh;
            *idx_ptr++ = rl;
        }
    }

    rd.ebo = rc.CreateBuffer(ur::INDEXBUFFER, indices.data(), sizeof(uint16_t) * indices.size());
    rd.num = indices.size();

    Block b;
    b.rd = rd;

    if (scale == 1)
    {
        b.trans.x = 1.0f / scale;
        b.trans.y = 1.0f / scale;
        b.trans.z = 0.5f;
        b.trans.w = 0.5f;
    }
    else
    {
        b.trans.x = 1.0f / scale;
        b.trans.y = 1.0f / scale;
        b.trans.z = 0.5f;
        b.trans.w = 0.5f;
    }

    return b;
}

}