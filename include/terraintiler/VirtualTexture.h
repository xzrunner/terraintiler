#pragma once

#include <memory>
#include <string>

namespace vtex { class VirtualTexture; class FeedbackBuffer; }
namespace textile { struct VTexInfo; }
namespace ur { class Device; }

namespace terraintiler
{

class VirtualTexture
{
public:
    VirtualTexture(const ur::Device& dev, const std::string& filepath);

    auto GetVTex() const { return m_vtex; }

private:
    std::shared_ptr<vtex::VirtualTexture> m_vtex = nullptr;

}; // VirtualTexture

}