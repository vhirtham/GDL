#pragma once

namespace GDL::GLSL
{


const char* GetLinearizeDepthShaderCode()
{
    return R"glsl(
            #version 450

            float LinearizeDepth(float depth)
            {
                float near = 0.1;
                float far = 100;
                float z = 2 * depth - 1;
                return (2.f * near) / (far + near -z * (far - near));
            }
            )glsl";
}
} // namespace GDL::GLSL
