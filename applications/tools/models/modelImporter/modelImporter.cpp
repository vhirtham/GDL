#include "src/openGLWidget.h"
#include "src/mainWindow.h"
#include "src/model.h"

#include "gdl/base/container/vector.h"
#include "gdl/base/string.h"
#include "gdl/math/vec3.h"
#include "gdl/math/transformations4.h"
#include "gdl/rendering/openGL/core/program.h"
#include "gdl/rendering/openGL/core/shader.h"



#include <QApplication>
#include <QOpenGLWidget>


using namespace GDL;



int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(1600, 900);
    OpenGLWidget ogl(&w);
    ogl.resize(1000, 800);
    w.show();


    // Shader Code ##############################

    const char vertexShaderCode[] = R"glsl(
            #version 430

            layout (location=0) in vec3 vertexPosition;
            layout (location=1) in vec3 vertexNormal;
            layout (location=2) in vec2 vertexTexCoord0;

            uniform mat4 projection;
            uniform mat4 modelWorld;

            out vec3 normal;
            out vec2 texCoord0;

            void main(void)
            {
                normal = vertexNormal;
                texCoord0 = vertexTexCoord0;
                gl_Position = projection * modelWorld * vec4(vertexPosition,1.0);
            }
            )glsl";



    const char fragmentShaderCode[] = R"glsl(
            #version 430

            float LinearDepth(float depth)
            {
                float near = 0.1;
                float far = 100;
                float z = 2 * depth - 1;
                return (2.f * near) / (far + near -z * (far - near));
            }


            uniform sampler2D texture0;

            in vec3 normal;
            in vec2 texCoord0;

            out vec4 FragColor;


            void main(void)
            {
                vec3 unitNormal = normalize(normal);

                vec4 materialAlbedo = texture(texture0, texCoord0) + vec4(texCoord0.x, texCoord0.y, 0,1);

                vec3 lightColor = vec3(1.f, 1.f, 1.f);
                vec3 lightDirection = -normalize(vec3(-1.f, 2.f, -1.f));

                vec3 ambient = lightColor * 1.f;
                float diff = max(dot(unitNormal, lightDirection), 0.f);
                vec3 diffuse =  lightColor * diff;

                FragColor = vec4((ambient ) * materialAlbedo.xyz, materialAlbedo.w);
            }
            )glsl";

    using namespace GDL;
    using namespace GDL::OpenGL;
    // Create Shader and Program ################

    Shader vertexShader(GL_VERTEX_SHADER, vertexShaderCode);
    Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
    Program program(vertexShader, fragmentShader);



    program.Use();

    F32 scale = 0.5f;

    program.SetUniformSamplerTextureUnit("texture0", 0);
    program.SetUniform(program.QueryUniformLocation("projection"),
                       Transformations4::PerspectiveProjection(45, 1000, 800, 0.1f, 100));
    program.SetUniform(program.QueryUniformLocation("modelWorld"),
                       Transformations4::Translation(0, -1, -7) *
                               Transformations4::Scale(scale, scale, scale));

    ogl.update();

    EXCEPTION(glGetError() != GL_NO_ERROR, "OpenGl Error");
    return a.exec();
}
