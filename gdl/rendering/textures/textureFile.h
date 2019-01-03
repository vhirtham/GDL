#pragma once



namespace GDL
{

class TextureData2d;



namespace TextureFile
{

//! @brief Reads data from a texture file
//! @param fileName: Name of the texture file
//! @return Texture data
TextureData2d Read(const char* fileName);

//! @brief Writes data from a texture file
//! @param fileName: Name of the texture file
//! @param textureData: Texture data
void Write(const char* fileName, const TextureData2d& textureData);

} // namespace TextureFile


} // namespace GDL
