static const char* quad_vs_src = R"(
#version 450

layout(location = 0) in vec3 vPos;

out vec2 uv;

void main()
{
   gl_Position = vec4(vPos, 1);
   uv = (vPos.xy + 1) * 0.5;
}
)";

static const char* quad_fs_src = R"(
#version 450

in vec2 uv;

uniform sampler2D tex;

out vec4 color;

void main()
{
  color = texture(tex, uv);
}
)";

class QuadShader
{
private:
    GLuint m_shader;
public:
    QuadShader() {
      auto vs = CompileShader(GL_VERTEX_SHADER, quad_vs_src);
      auto fs = CompileShader(GL_FRAGMENT_SHADER, quad_fs_src);
      m_shader = GenerateProgram(vs, fs);
    }

    void use(GLuint texture) const
    {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);
      glUseProgram(m_shader);
    }

    static int SH_IN_VPOS;
};

int QuadShader::SH_IN_VPOS = 0;