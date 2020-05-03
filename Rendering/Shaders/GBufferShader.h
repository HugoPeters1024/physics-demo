static const char* gbuffer_vs_src = R"(
#version 450

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUv;

layout(location = 0) uniform mat4 uCamera;
layout(location = 1) uniform mat4 uMvp;

out vec3 fragNormal;
out vec3 fragPos;
out vec2 uv;

void main()
{
    vec4 worldPos = uMvp * vec4(vPos, 1.0);
    fragPos = worldPos.xyz;
    fragNormal = normalize((uMvp * vec4(vNormal,0)).xyz);
    uv = vUv;
    gl_Position = uCamera * worldPos;
}
)";

static const char* gbuffer_fs_src = R"(
#version 450

in vec3 fragNormal;
in vec3 fragPos;
in vec2 uv;

layout(location = 2) uniform sampler2D diffuse;
layout(location = 3) uniform sampler2D lighting;

layout(location = 0) out vec3 o_normal;
layout(location = 1) out vec3 o_pos;
layout(location = 2) out vec3 o_albedo;
layout(location = 3) out vec3 o_lighting;

void main() {
  o_normal = fragNormal;
  o_pos = fragPos;
  o_albedo = texture(diffuse, uv).xyz;
  o_lighting = texture(lighting, uv).xyz;
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

    void prepare(const Camera::Camera* camera) const {
      glUseProgram(m_program);

      mat4x4 e_camera;
      camera->getMatrix().unpack(e_camera);
      glUniformMatrix4fv(SH_UN_CAMERA, 1, GL_FALSE, (const GLfloat*)e_camera);
    }

    void use(const Matrix4& mvp, GMaterial material) const {
      mat4x4 e_mvp;
      mvp.unpack(e_mvp);
      glUniformMatrix4fv(SH_UN_MVP, 1, GL_FALSE, (const GLfloat*)e_mvp);

      glUniform1i(SH_UN_DIFFUSE, 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, material.m_diffuse);

      glUniform1i(SH_UN_LIGHTING, 1);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, material.m_lighting);
    }

    static int SH_IN_VPOS;
    static int SH_IN_VNORMAL;
    static int SH_IN_VUV;

    static int SH_UN_CAMERA;
    static int SH_UN_MVP;
    static int SH_UN_DIFFUSE;
    static int SH_UN_LIGHTING;
};

int GBufferShader::SH_IN_VPOS = 0;
int GBufferShader::SH_IN_VNORMAL = 1;
int GBufferShader::SH_IN_VUV = 2;

int GBufferShader::SH_UN_CAMERA = 0;
int GBufferShader::SH_UN_MVP = 1;
int GBufferShader::SH_UN_DIFFUSE = 2;
int GBufferShader::SH_UN_LIGHTING = 3;
