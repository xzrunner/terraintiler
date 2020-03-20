#include "terraintiler/VirtualTexture.h"

#include <textile/VTexInfo.h>
#include <textile/TileDataFile.h>
#include <vtex/VirtualTexture.h>

namespace
{

const int FEEDBACK_SIZE = 64;

}

namespace terraintiler
{

VirtualTexture::VirtualTexture(const std::string& filepath)
{
    textile::VTexInfo info;
    std::fstream fin(filepath.c_str(), std::ios::in | std::ios::binary);
    textile::TileDataFile::ReadHeader(info, fin);
    fin.close();

    m_vtex = std::make_shared<vtex::VirtualTexture>(filepath, info, info.channels, FEEDBACK_SIZE);
}

}