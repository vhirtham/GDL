#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

#include "gdl/base/exception.h"
#include "gdl/base/string.h"
#include "gdl/resources/memory/memoryManager.h"
#include "gdl/rendering/textures/textureData.h"
#include "gdl/rendering/textures/textureFile.h"

// reminder: Paint clone ---> Pinta

#include <experimental/filesystem>
#include <iostream>
#include <vector>



using namespace GDL;
namespace filesystem = std::experimental::filesystem;


int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    MemoryManager& memoryManager = MemoryManager::Instance();
    memoryManager.CreateGeneralPurposeMemory(50_MB);
    memoryManager.Initialize();

    if (!filesystem::exists("input"))
    {
        bool success = filesystem::create_directory("input");
        EXCEPTION(!success, "Could not create input directory");
    }
    if (!filesystem::exists("output"))
    {
        bool success = filesystem::create_directory("output");
        EXCEPTION(!success, "Could not create output directory");
    }


    stbi_set_flip_vertically_on_load(true);
    for (const auto& entry : filesystem::directory_iterator("input"))
    {
        String fileName{entry.path().filename().c_str()};
        String filePath{entry.path().c_str()};
        String convertedName{MakeString(entry.path().stem().c_str(), ".tex")};
        String convertedRelPath{MakeString("output/", convertedName)};
        std::cout << "Converting \"" << entry.path().filename().c_str() << "\" to \"" << convertedName << "\""
                  << std::endl;

        I32 width, height, channels;

        U8* pixelData = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_default);
        EXCEPTION(pixelData == nullptr, MakeString("Failed to load image:\n", filePath).c_str());
        TextureData textureData(static_cast<U32>(width), static_cast<U32>(height), static_cast<U32>(channels),
                                pixelData);

        TextureFile::Write(convertedRelPath.c_str(), textureData);

        stbi_image_free(pixelData);
    }

    memoryManager.Deinitialize();
    return 0;
}
