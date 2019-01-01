#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

#ifndef DATA_DIRECTORY
#define DATA_DIRECTORY "data"
#endif


// reminder: Paint clone ---> Pinta

#include <iostream>
#include <cstring>
#include <vector>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    int width, height, channels;

    std::string fileName{DATA_DIRECTORY};
    fileName.append("/testImage.png");

    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load(fileName.c_str(), &width, &height, &channels, STBI_rgb);

    unsigned int dataSize = static_cast<unsigned int>(width * height * channels);
    std::vector<unsigned char> data(dataSize);
    std::memcpy(data.data(), image, dataSize);

    stbi_image_free(image);

    std::cout << width << " / " << height << std::endl;
    return 0;
}
