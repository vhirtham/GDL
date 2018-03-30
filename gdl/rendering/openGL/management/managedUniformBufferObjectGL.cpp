#include "gdl/rendering/openGL/management/managedUniformBufferObjectGL.h"


#include "gdl/base/Exception.h"


#include <cassert>

GDL::OpenGL::ManagedUniformBufferObject::ManagedUniformBufferObject(const UniformBlock& uniformBlock, GLenum usage,
                                                                    const std::vector<U8>& buffer)
    : mUBO(buffer, usage)
    , mVariables(uniformBlock.GetVariables())
{
    if (static_cast<GLsizei>(buffer.size()) != uniformBlock.GetSize())
        throw Exception(
                __PRETTY_FUNCTION__,
                "Dimension of buffer which is provided for initialization does not fit the uniform blocks size!");
    Initialize(uniformBlock);
}

GDL::OpenGL::ManagedUniformBufferObject::ManagedUniformBufferObject(const UniformBlock& uniformBlock, GLenum usage)
    : mUBO(uniformBlock.GetSize(), usage)
    , mVariables(uniformBlock.GetVariables())
{
    Initialize(uniformBlock);
}

const GDL::OpenGL::UniformBlockVariable&
GDL::OpenGL::ManagedUniformBufferObject::GetVariable(std::string variableName) const
{
    auto iterator = mVariables.find(variableName);
    if (iterator == mVariables.end())
        throw Exception(__PRETTY_FUNCTION__,
                        "Did not find uniform \"" + variableName +
                                "\". GLSL compiler optimization might be the reason.");
    return iterator->second;
}


bool GDL::OpenGL::ManagedUniformBufferObject::CheckUniformBlockCompatibility(const UniformBlock& uniformBlock) const
{
    if (mUBO.GetSize() != uniformBlock.GetSize() || mVariables.size() != uniformBlock.GetNumVariables())
        return false;

    for (const auto& variableBlock : uniformBlock.GetVariables())
    {
        const auto& variableBuffer = mVariables.find(variableBlock.first);
        if (variableBuffer == mVariables.end() ||
            !(variableBuffer->second.CheckEqualDataStructure(variableBlock.second)))
            return false;
    }

    return true;
}

void GDL::OpenGL::ManagedUniformBufferObject::Initialize(const UniformBlock& uniformBlock)
{
    assert(mUBO.GetSize() == uniformBlock.GetSize());
    assert(CheckUniformBlockCompatibility(uniformBlock));
    SetBindingPoint(uniformBlock.GetBindingPoint());
}
