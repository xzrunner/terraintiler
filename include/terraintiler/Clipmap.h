#pragma once

#include "Renderable.h"

#include <SM_Rect.h>
#include <unirender/Texture.h>

#include <array>
#include <vector>

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
    };

public:
    Clipmap();

    auto& GetAllLayers() const { return m_layers; }

private:
    void Build();

    static Block BuildBlock(const sm::rect& region,
        size_t resolution, float scale);

private:
    static const size_t LAYER_NUM = 10;

private:
    std::array<Layer, LAYER_NUM> m_layers;

}; // Clipmap

}