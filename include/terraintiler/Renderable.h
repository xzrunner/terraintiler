#pragma once

namespace ur2 { class VertexArray; }

#include <memory>

namespace terraintiler
{

struct Renderable
{
    //size_t vbo = 0, ebo = 0;
    //size_t num = 0;

    std::shared_ptr<ur2::VertexArray> va = nullptr;
};

}