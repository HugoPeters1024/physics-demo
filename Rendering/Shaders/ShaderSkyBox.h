static const char* skybox_vs_src = R"(
#version 450

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUv;

layout(location = 0) uniform mat4 uCamera;
layout(location = 1) uniform mat4 uMvp;

out vec3 fragNormal;
out vec3 fragPos;
out vec3 uv;

void main()
{
    vec4 worldPos = uMvp * vec4(vPos, 1.0);
    fragPos = worldPos.xyz;
    fragNormal = normalize((uMvp * vec4(vNormal,0)).xyz);
    uv = vPos;
    gl_Position = uCamera * worldPos;
}
)";

static const char* skybox_fs_src = R"(
#version 450

in vec3 fragNormal;
in vec3 fragPos;
in vec3 uv;

layout(location = 2) uniform samplerCube tex;
layout(location = 0) out vec3 o_color;

void main() {
  o_color = texture(tex, uv).xyz;
}
)";


class ShaderSkyBox
{
private:
    GLuint m_program;
public:
    ShaderSkyBox()
    {
      auto vs = CompileShader(GL_VERTEX_SHADER, skybox_vs_src);
      auto fs = CompileShader(GL_FRAGMENT_SHADER, skybox_fs_src);
      m_program = GenerateProgram(vs, fs);
    }

    void use(const Camera::Camera* camera, const Matrix4& mvp, GLuint texture) const {
      glUseProgram(m_program);

      mat4x4 e_camera;
      camera->getMatrix().unpack(e_camera);
      glUniformMatrix4fv(SH_UN_CAMERA, 1, GL_FALSE, (const GLfloat*)e_camera);

      mat4x4 e_mvp;
      mvp.unpack(e_mvp);
      glUniformMatrix4fv(SH_UN_MVP, 1, GL_FALSE, (const GLfloat*)e_mvp);

      glUniform1i(SH_UN_TEX, 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    }

    static int SH_IN_VPOS;
    static int SH_IN_VNORMAL;
    static int SH_IN_VUV;

    static int SH_UN_CAMERA;
    static int SH_UN_MVP;
    static int SH_UN_TEX;
};

int ShaderSkyBox::SH_IN_VPOS = 0;
int ShaderSkyBox::SH_IN_VNORMAL = 1;
int ShaderSkyBox::SH_IN_VUV = 2;

int ShaderSkyBox::SH_UN_CAMERA = 0;
int ShaderSkyBox::SH_UN_MVP = 1;
int ShaderSkyBox::SH_UN_TEX = 2;
