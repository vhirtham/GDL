#pragma once



namespace GDL
{

class TextureData;



namespace TextureFile
{

//! @brief Reads data from a texture file
//! @param fileName: Name of the texture file
//! @return Texture data
TextureData Read(const char* fileName);

//! @brief Writes data from a texture file
//! @param fileName: Name of the texture file
//! @param textureData: Texture data
void Write(const char* fileName, const TextureData& textureData);

} // namespace TextureFile


} // namespace GDL
