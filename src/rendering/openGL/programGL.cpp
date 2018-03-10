#include "programGL.h"

#include "base/Exception.h"

#include <cassert>
#include <memory>

GDL::ProgramGL::~ProgramGL()
{
    glDeleteProgram(mHandle);
}



GDL::ProgramGL::ProgramGL(std::initializer_list<std::reference_wrapper<const ShaderGLSL>> shaderList)
{
    Initialize(shaderList);
}

GLuint GDL::ProgramGL::GetUniformHandle(std::string uniformName)
{
    auto iterator = mUniforms.find(uniformName);
#ifndef NDEBUG
    if (iterator == mUniforms.end())
        throw Exception(__PRETTY_FUNCTION__,
                        "Did not find uniform \"" + uniformName +
                                "\". GLSL compiler optimization might be the reason.");
#endif
    return iterator->second.GetHandle();
}


template <>
void GDL::ProgramGL::SetUniformScalar(GLuint uniformHandle, F32 value, GLsizei index)
{
    std::vector<GLfloat> values(index);
    for (U32 i = 0; i < index; ++i)
        values[i] = value;
    assert(GetUniformByHandle(uniformHandle).GetType() == GL_FLOAT);
    glProgramUniform1fv(mHandle, uniformHandle, index, values.data());
}

void GDL::ProgramGL::CheckLinkStatus()
{
    GLint status;
    glGetProgramiv(mHandle, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &infoLogLength);

        std::unique_ptr<GLchar[]> infoLog = std::make_unique<GLchar[]>(infoLogLength);
        glGetProgramInfoLog(mHandle, infoLogLength, nullptr, infoLog.get());

        std::string errorMessage(infoLog.get());
        throw Exception(__PRETTY_FUNCTION__, "Failed to compile shader program \n\nLog Message:\n" + errorMessage);
    }
}

void GDL::ProgramGL::Initialize(std::initializer_list<std::reference_wrapper<const GDL::ShaderGLSL>> shaderList)
{
    mHandle = glCreateProgram();
    for (auto& shader : shaderList)
    {
        assert(mShader.find(shader.get().GetType()) == mShader.end() &&
               "Multiple shaders of the same type in one program are not supported");

        auto emplaceResult = mShader.emplace(shader.get().GetType(), shader);
        assert(emplaceResult.second == true && "Could not emplace shader into program data");

        glAttachShader(mHandle, emplaceResult.first->second.get().GetHandle());
    }

    glLinkProgram(mHandle);

    for (auto& shader : mShader)
    {
        glDetachShader(mHandle, shader.second.get().GetHandle());
    }
    CheckLinkStatus();
    FindInputs();
    FindUniforms();
    FindUniformBlocks();
    GLenum errorCode = glGetError();
    if (errorCode != GL_NO_ERROR)
        throw Exception(__PRETTY_FUNCTION__,
                        "Could not create program:\n" +
                                std::string(reinterpret_cast<const char*>(gluErrorString(errorCode))));
}



void GDL::ProgramGL::FindInputs()
{
    auto inputData = FindProgramResourceData<2>(GL_PROGRAM_INPUT, {{GL_LOCATION, GL_NAME_LENGTH}});
    for (U32 i = 0; i < inputData.size(); ++i)
        mProgramInputs.emplace(GetResourceName(GL_PROGRAM_INPUT, i, inputData[i][1]), inputData[i][0]);
}



void GDL::ProgramGL::FindUniforms()
{
    auto uniformData = FindProgramResourceData<5>(
            GL_UNIFORM, {{GL_LOCATION, GL_NAME_LENGTH, GL_TYPE, GL_ARRAY_SIZE, GL_BLOCK_INDEX}});
    for (U32 i = 0; i < uniformData.size(); ++i)
    {
        if (uniformData[i][4] != -1) // if uniform is member of a UBO
            continue;
        //        if (uniformData[i][3] > 1)
        //            // READ:
        //            //
        //            https://stackoverflow.com/questions/32154710/are-glgetuniformlocation-indices-for-arrays-of-uniforms-guaranteed-sequential-ex
        //            // Possibilities:
        //            // 1. Keep size in uniform to store array size -> Duplicate entries (or decending to the end. Last
        //            element
        //            // should have size=1)
        //            // 2. Store array data in an extra container -> unnecessary complexity
        //            // 3. Count number of entries in uniform map -> slow, but you should do that only once and
        //            remember the
        //            // value
        //            // Currently favour version 1 because version 2 gets to messy and with version 3. you need to hope
        //            always
        //            // remembering that this solution is slow. If you forget it, you might slow the program down!
        //            // Copy these thoughts to github in a new issue and reference it above the function to always
        //            remember the
        //            // reasons for the decision.
        //            throw Exception(__PRETTY_FUNCTION__,
        //                            "Arrays not handled yet. Add each arrayelemet to the uniforms list. "
        //                            "Think about how to get the size. Store an extra element or just search for the
        //                            number of "
        //                            "elements with the same name in the UniformList");
        mUniforms.emplace(GetResourceName(GL_UNIFORM, i, uniformData[i][1]),
                          Uniform(uniformData[i][0], uniformData[i][2], uniformData[i][3]));
    }
}

void GDL::ProgramGL::FindUniformBlocks()
{
    auto uniformData = FindProgramResourceData<4>(
            GL_UNIFORM_BLOCK, {{GL_NAME_LENGTH, GL_BUFFER_BINDING, GL_BUFFER_DATA_SIZE, GL_NUM_ACTIVE_VARIABLES}});
    for (U32 i = 0; i < uniformData.size(); ++i)
    {
        std::string uniformBlockName = GetResourceName(GL_UNIFORM_BLOCK, i, uniformData[i][0]);
        GLint bufferBindingPoint = uniformData[i][0];
        assert(glGetUniformBlockIndex(mHandle, uniformBlockName.c_str()) == i);

        std::vector<GLint> varIndices(uniformData[i][3]);
        glGetActiveUniformBlockiv(mHandle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, varIndices.data());
        for (U32 j = 0; j < uniformData[i][3]; ++j)
        {
            GLuint uniformIndex = static_cast<GLuint>(varIndices[j]);
            GLint uniformNameLength;
            glGetActiveUniformsiv(mHandle, 1, &uniformIndex, GL_UNIFORM_NAME_LENGTH, &uniformNameLength);
            std::string uniformName = GetResourceName(GL_UNIFORM, uniformIndex, uniformNameLength);
            GLint uniformOffset;
            glGetActiveUniformsiv(mHandle, 1, &uniformIndex, GL_UNIFORM_OFFSET, &uniformOffset);
            GLint uniformSize;
            glGetActiveUniformsiv(mHandle, 1, &uniformIndex, GL_UNIFORM_SIZE, &uniformSize);
            int a = 0;
        }
    }
}



std::string GDL::ProgramGL::GetResourceName(GLenum eResourceType, GLuint index, GLint bufferSize)
{
    std::unique_ptr<GLchar[]> resourceName = std::make_unique<GLchar[]>(bufferSize);
    glGetProgramResourceName(mHandle, eResourceType, index, bufferSize, nullptr, resourceName.get());
    std::string nameString(resourceName.get());
    return nameString;
}

#ifndef NDEBUG
const GDL::Uniform& GDL::ProgramGL::GetUniformByHandle(GLuint handle)
{
    for (const auto& uniform : mUniforms)
        if (uniform.second.GetHandle() == handle)
            return uniform.second;

    throw Exception(__PRETTY_FUNCTION__, "Uniform not found!");
}
#endif


template <GDL::U32 TNumProps>
std::vector<std::array<GLint, TNumProps>>
GDL::ProgramGL::FindProgramResourceData(GLenum eResourceType, std::array<GLenum, TNumProps> properties)
{
    GLint numResources = 0;
    glGetProgramInterfaceiv(mHandle, eResourceType, GL_ACTIVE_RESOURCES, &numResources);
    std::vector<std::array<GLint, TNumProps>> results(numResources);
    for (GLint i = 0; i < numResources; ++i)
    {
        glGetProgramResourceiv(mHandle, eResourceType, i, TNumProps, properties.data(), TNumProps, nullptr,
                               results[i].data());
    }
    return results;
}
