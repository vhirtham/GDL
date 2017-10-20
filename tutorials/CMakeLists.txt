
set(VULKAN_SDK_PATH /home/volker/cpp/VulkanSDK/1.0.61.1/x86_64)

INCLUDE_DIRECTORIES(${VULKAN_SDK_PATH}/include)
LINK_DIRECTORIES(${VULKAN_SDK_PATH}/lib)

add_executable(VulkanTest vulkan/01/vulkanTutorial01.cpp)
target_link_libraries(VulkanTest
    -lglfw3
    -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp
    -lvulkan)
