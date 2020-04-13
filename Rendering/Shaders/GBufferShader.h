static const char* gbuffer_vs_src = R"(
#version 450

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

layout(location = 0) uniform mat4 uCamera;
layout(location = 1) uniform mat4 uMvp;

out vec3 fragNormal;
out vec3 fragPos;

void main()
{
    vec4 worldPos = uMvp * vec4(vPos, 1.0);
    fragPos = worldPos.xyz;
    fragNormal = normalize((uMvp * vec4(vNormal,0)).xyz);
    gl_Position = uCamera * worldPos;
}
)";

static const char* gbuffer_fs_src = R"(
#version 450

in vec3 fragNormal;
in vec3 fragPos;

layout(location = 0) out vec3 o_normal;
layout(location = 1) out vec3 o_pos;

void main() {
  o_normal = fragNormal;
  o_pos = fragPos;
}
)";


class GBufferShader
{
private:
    GLuint m_program;
public:
    GBufferShader()
    {
      auto vs = CompileShader(GL_VERTEX_SHADER, gbuffer_vs_src);
      auto fs = CompileShader(GL_FRAGMENT_SHADER, gbuffer_fs_src);
      m_program = GenerateProgram(vs, fs);
    }

    void use(const Camera::Camera* camera, const Matrix4& mvp, const Material& material) const {
      glUseProgram(m_program);

      mat4x4 e_camera;
      camera->getMatrix().unpack(e_camera);
      glUniformMatrix4fv(SH_UN_CAMERA, 1, GL_FALSE, (const GLfloat*)e_camera);

      mat4x4 e_mvp;
      mvp.unpack(e_mvp);
      glUniformMatrix4fv(SH_UN_MVP, 1, GL_FALSE, (const GLfloat*)e_mvp);

      Vector3 camPos = camera->getPosition();
      glUniform3f(SH_UN_CAMERA_POS, camPos.x, camPos.y, camPos.z);

      glUniform1f(SH_UN_TIME, glfwGetTime());

      glUniform3f(SH_UN_MATERIAL_COLOR, material.diffuseColor.x, material.diffuseColor.y, material.diffuseColor.z);
      glUniform1f(SH_UN_MATERIAL_SPECULAR, material.specular);
    }

    static int SH_IN_VPOS;
    static int SH_IN_VNORMAL;

    static int SH_UN_CAMERA;
    static int SH_UN_MVP;
    static int SH_UN_CAMERA_POS;
    static int SH_UN_TIME;
    static int SH_UN_MATERIAL_COLOR;
    static int SH_UN_MATERIAL_SPECULAR;
};

int GBufferShader::SH_IN_VPOS = 0;
int GBufferShader::SH_IN_VNORMAL = 1;

int GBufferShader::SH_UN_CAMERA = 0;
int GBufferShader::SH_UN_MVP = 1;
int GBufferShader::SH_UN_CAMERA_POS = 2;
int GBufferShader::SH_UN_TIME = 3;
int GBufferShader::SH_UN_MATERIAL_COLOR = 4;
int GBufferShader::SH_UN_MATERIAL_SPECULAR = 5;
