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



//! @brief Class that stores all relevant data to create a texture
class TextureData2d
{
    friend TextureData2d GDL::TextureFile::Read(const char*);


    U32 mWidth;
    U32 mHeight;
    U32 mNumChannels;
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

    //! @brief Gets the height of the texture
    //! @return Height of the texture
    U32 GetHeight() const;

    //! @brief Gets the number of channels of the texture
    //! @return Number of channels of the texture
    U32 GetNumChannels() const;

    //! @brief Gets the pixel data of the specified texture level
    //! @param level: Texture level.
    //! @return Pixel data of the texture
    const Vector<U8>& GetPixelData(U32 level = 0) const;

    //! @brief Gets the width of the texture
    //! @return Width of the texture
    U32 GetWidth() const;

    //! @brief Checks if the internally storid data is valid and consistent
    //! @return True / False
    bool IsDataValid() const;
};

} // namespace GDL
