#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/container/vector.h"


namespace GDL
{

class TextureData2d;

namespace TextureFile
{

TextureData2d Read(const char*);

} // namespace TextureFile



enum class Interpolation
{
    NONE = 0,
    LINEAR
};


//! @brief Class that stores all relevant data to create a texture
class TextureData2d
{
    friend TextureData2d GDL::TextureFile::Read(const char*);


    U32 mNumChannels;
    std::vector<U32> mWidth;
    std::vector<U32> mHeight;
    std::vector<Vector<U8>> mPixelData;


    //! @brief ctor
    //! @remark: It is private since only the TextureFile::Read function should use this ctor
    TextureData2d();

public:
    TextureData2d(const TextureData2d&) = default;
    TextureData2d(TextureData2d&&) = default;
    TextureData2d& operator=(const TextureData2d&) = default;
    TextureData2d& operator=(TextureData2d&&) = default;
    ~TextureData2d() = default;

    //! @brief Ctor
    //! @param width: Texture width
    //! @param height: Texture height
    //! @param numChannels: Number of date channels
    //! @param data: Texture data as array of bytes
    TextureData2d(U32 width, U32 height, U32 numChannels, const Vector<U8>& data);

    //! @brief Ctor
    //! @param width: Texture width
    //! @param height: Texture height
    //! @param numChannels: Number of date channels
    //! @param data: Texture data as array of bytes
    TextureData2d(U32 width, U32 height, U32 numChannels, const U8* data);

    //! @brief Creates mipmaps for the texture
    //! @param interpolation: Interpolation method to create the mip maps. If NONE is chosen the whole pixeldata of the
    //! mipmaps is set to 0.
    void CreateMipMaps(Interpolation interpolation = Interpolation::LINEAR);

    //! @brief Gets the height of the texture
    //! @return Height of the texture
    U32 GetHeight(U32 level = 0) const;

    //! @brief Gets the number of channels of the texture
    //! @return Number of channels of the texture
    U32 GetNumChannels() const;

    //! @brief Gets the number of texture levels
    //! @return Number of texture levels;
    U32 GetNumTextureLevels() const;

    //! @brief Gets the pixel data of the specified texture level
    //! @param level: Texture level.
    //! @return Pixel data of the texture
    const Vector<U8>& GetPixelData(U32 level = 0) const;

    //! @brief Gets the width of the texture
    //! @return Width of the texture
    U32 GetWidth(U32 level = 0) const;

    //! @brief Checks if the internally storid data is valid and consistent
    //! @return True / False
    bool IsDataValid() const;

private:
    //! @brief Calculates a mipmap for the chosen level by linear interpolation of the previous level
    //! @param level: Level of the mipmap
    void CalculateLinearMipMap(U32 level);
};

} // namespace GDL
