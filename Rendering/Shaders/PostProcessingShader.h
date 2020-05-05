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
layout(location = 2) uniform float cameraSpeed;

out vec4 color;

void main()
{
  // widescreen
  if (uv.y < 0.1 || uv.y > 0.9) return;

  vec2 fromcenter = (uv - vec2(0.5));
  vec2 abberation = fromcenter * 0.002;

  float disfromcenter = dot(fromcenter*2, fromcenter*2);

  float corr = 0.1f;
  for(int i=0; i<10; i++)
  {
    float f = i * corr;
    vec2 offset = fromcenter * cameraSpeed * 0.01 * f;
    color.r += texture(tex, uv + abberation * 0 + offset).r * corr;
    color.g += texture(tex, uv + abberation * 1 + offset).g * corr;
    color.b += texture(tex, uv + abberation * 2 + offset).b * corr;
  }
  color *= (1-disfromcenter*0.6);
  color = color * 1.1 - 0.1;
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

    void prepare(float camSpeed) const {
      glUseProgram(m_program);
      glUniform1f(SH_UN_CAMSPEED, camSpeed);
    }

    void use(GLuint texture) const {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);

      glUniform1f(SH_UN_TIME, glfwGetTime());
    }

    static int SH_UN_TIME;
    static int SH_UN_CAMSPEED;
};

int PostProcessingShader::SH_UN_TIME = 1;
int PostProcessingShader::SH_UN_CAMSPEED = 2;
