static const char* post_vs_src = R"(
#version 450

layout(location = 0) in vec3 vPos;

out vec2 uv;

void main()
{
   gl_Position = vec4(vPos, 1);
   float scale = 0.98;
   uv = vPos.xy * 0.5 * scale + 0.5;
}
)";

static const char* post_fs_src = R"(
#version 450

in vec2 uv;

layout(location = 0) uniform sampler2D tex;
layout(location = 1) uniform float iTime;

out vec4 color;

void main()
{
  // widescreen
  if (uv.y < 0.1 || uv.y > 0.9) return;

  vec2 fromcenter = (uv - vec2(0.5));
  vec2 abberation = fromcenter * 0.002;

  float disfromcenter = dot(fromcenter*2, fromcenter*2);

  color.r = texture(tex, uv + abberation * 0).r;
  color.g = texture(tex, uv + abberation * 1).g;
  color.b = texture(tex, uv + abberation * 2).b;
  color *= (1-disfromcenter*0.6);
}
)";

class PostProcessingShader : public IQuadShader {
private:
    GLuint m_program;
public:
    PostProcessingShader() {
      GLuint vs = CompileShader(GL_VERTEX_SHADER, post_vs_src);
      GLuint fs = CompileShader(GL_FRAGMENT_SHADER, post_fs_src);
      m_program = GenerateProgram(vs, fs);
    }

    void use(GLuint texture) const override {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);
      glUseProgram(m_program);

      glUniform1f(SH_UN_TIME, glfwGetTime());
    }

    static int SH_UN_TIME;
};

int PostProcessingShader::SH_UN_TIME = 1;
