#pragma once

#include "Renderable.h"

#include <SM_Rect.h>
#include <unirender/Texture.h>

#include <array>
#include <vector>

namespace clipmap { class Clipmap; }

//#define HEIGHT_MAP_PCG

namespace terraintiler
{

class Clipmap
{
public:
    struct Block
    {
        Renderable rd;
        sm::vec4 trans;
    };

    struct Layer
    {
        // level0 16, others 12
        std::vector<Block> blocks;

        std::shared_ptr<ur::Texture> heightmap = nullptr;
        sm::vec4 uv_region = sm::vec4(0, 0, 1, 1);
    };

public:
    Clipmap(const std::string& vtex_path);

    auto& GetAllLayers() const { return m_layers; }

    void Update(float scale, const sm::vec2& offset);
    void DebugDraw() const;

    auto GetVTex() const { return m_vtex; }

private:
#ifndef HEIGHT_MAP_PCG
    void InitVTex(const std::string& vtex_path);
#endif // HEIGHT_MAP_PCG

    void Build();

    static Block BuildBlock(const sm::rect& region,
        size_t resolution, float scale);

private:
    static const size_t LAYER_NUM = 10;

private:
    std::array<Layer, LAYER_NUM> m_layers;

    mutable std::shared_ptr<clipmap::Clipmap> m_vtex = nullptr;

}; // Clipmap

}