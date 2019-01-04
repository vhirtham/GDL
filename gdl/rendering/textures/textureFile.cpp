#include "gdl/rendering/textures/textureFile.h"

#include "gdl/base/exception.h"
#include "gdl/base/string.h"
#include "gdl/rendering/textures/textureData2d.h"

#include <fstream>

namespace GDL::TextureFile
{



void ReadPixelData(std::ifstream& textureFile, Vector<U8>& data, U32 numValues)
{
    data.resize(numValues);
    textureFile.read(reinterpret_cast<char*>(data.data()), static_cast<I32>(numValues * sizeof(U8)));
}



void ReadU32(std::ifstream& textureFile, U32& value)
{
    textureFile.read(reinterpret_cast<char*>(&value), sizeof(U32));
}



void WritePixelData(std::ofstream& textureFile, const Vector<U8>& data)
{
    textureFile.write(reinterpret_cast<const char*>(data.data()), static_cast<I32>(data.size() * sizeof(U8)));
}



void WriteU32(std::ofstream& textureFile, U32 value)
{
    textureFile.write(reinterpret_cast<const char*>(&value), sizeof(U32));
}



TextureData2d Read(const char* fileName)
{
    std::ifstream textureFile(fileName, std::ifstream::in | std::ifstream::binary);
    EXCEPTION(!textureFile.is_open(), MakeString("Could not open output texture file:\n", fileName).c_str());

    TextureData2d textureData;

    ReadU32(textureFile, textureData.mWidth[0]);
    ReadU32(textureFile, textureData.mHeight[0]);
    ReadU32(textureFile, textureData.mNumChannels);

    textureData.mPixelData.emplace_back();
    ReadPixelData(textureFile, textureData.mPixelData[0],
                  textureData.mWidth[0] * textureData.mHeight[0] * textureData.mNumChannels);

    textureFile.close();

    EXCEPTION(!textureData.IsDataValid(), MakeString("Data of output texture file is corrupted:\n", fileName).c_str());

    return textureData;
}



void Write(const char* fileName, const TextureData2d& textureData)
{
    std::ofstream textureFile(fileName, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
    EXCEPTION(!textureFile.is_open(), MakeString("Could not open output texture file:\n", fileName).c_str());

    WriteU32(textureFile, textureData.GetWidth());
    WriteU32(textureFile, textureData.GetHeight());
    WriteU32(textureFile, textureData.GetNumChannels());
    WritePixelData(textureFile, textureData.GetPixelData());

    textureFile.close();
}



} // namespace GDL::TextureFile
