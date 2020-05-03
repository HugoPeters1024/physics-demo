static const char* lighting_skybox_vs_src = R"(
#version 450

layout(location = 0) in vec3 vPos;

out vec2 uv;

void main()
{
   gl_Position = vec4(vPos, 1);
   uv = (vPos.xy + 1) * 0.5;
}
)";

static const char* lighting_skybox_fs_src = R"(
      #version 450

      layout(location = 2) uniform vec3 uCamPos;
      layout(location = 3) uniform float uTime;

      layout(location = 4) uniform sampler2D posTex;
      layout(location = 5) uniform sampler2D normalTex;
      layout(location = 6) uniform sampler2D albedoTex;
      layout(location = 7) uniform sampler2D lightingTex;

      layout(location = 9) uniform vec2 screenSize;
      layout(location = 11) uniform vec3 lightCol;
      layout(location = 12) uniform samplerCube skyboxTex;
      layout(location = 13) uniform float lightness;


      out vec4 color;

      vec3 sampleSkybox(vec3 coords) {
          return texture(skyboxTex, coords).xyz * 2 * lightness;
      }

      void main()
      {
          vec2 uv = gl_FragCoord.xy / screenSize;
          vec3 fragPos = texture(posTex, uv).xyz;
          vec3 fragNormal = normalize(texture(normalTex, uv).xyz);
          vec3 fragColor = texture(albedoTex, uv).xyz;

          // Fragment is outside the world
          if (dot(fragNormal, fragNormal) < 0.99) {
              color = vec4(0);
              return;
          }

          vec3 lightingValues = texture(lightingTex, uv).rgb;

          vec3 eye = normalize(fragPos - uCamPos);
          vec3 refl = normalize(reflect(eye, fragNormal));

          vec3 reflectedComponent = sampleSkybox(refl) * lightingValues.y;

          float ratio = 1.00 / 1.52;
          vec3 refractedRay = refract(eye, fragNormal, ratio);
          vec3 refractedComponent = sampleSkybox(refractedRay) * lightingValues.z;

          color = clamp(vec4(reflectedComponent + refractedComponent + fragColor * lightness, 1), 0, 1);
      }
    )";

class LightingSkyboxShader {
private:
    GLuint m_program;
public:
    LightingSkyboxShader() {
      GLuint vs = CompileShader(GL_VERTEX_SHADER, lighting_skybox_vs_src);
      GLuint fs = CompileShader(GL_FRAGMENT_SHADER, lighting_skybox_fs_src);
      m_program = GenerateProgram(vs, fs);
    }
    void use(const Vector2& screenSize, const Camera::Camera* camera, const Matrix4& mvp, const GBuffer* gbuffer, GLuint skyboxTex, float lightness) const {
      glUseProgram(m_program);

      glUniform2f(SH_UN_SCREENSIZE, screenSize.x, screenSize.y);

      mat4x4 e_camera;
      camera->getMatrix().unpack(e_camera);
      glUniformMatrix4fv(SH_UN_CAMERA, 1, GL_FALSE, (const GLfloat*)e_camera);

      mat4x4 e_mvp;
      mvp.unpack(e_mvp);
      glUniformMatrix4fv(SH_UN_MVP, 1, GL_FALSE, (const GLfloat*)e_mvp);

      Vector3 camPos = camera->getPosition();
      glUniform3f(SH_UN_CAMERA_POS, camPos.x, camPos.y, camPos.z);

      glUniform1f(SH_UN_TIME, glfwGetTime());

      glUniform1i(SH_UN_POSTEX, 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, gbuffer->getPositionTexture());

      glUniform1i(SH_UN_NORMALTEX, 1);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, gbuffer->getNormalTexture());

      glUniform1i(SH_UN_ALBEDOTEX, 2);
      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_2D, gbuffer->getAlbedoTexture());

      glUniform1i(SH_UN_SKYBOXTEX, 3);
      glActiveTexture(GL_TEXTURE3);
      glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

      glUniform1i(SH_UN_LIGHTINGTEX, 4);
      glActiveTexture(GL_TEXTURE4);
      glBindTexture(GL_TEXTURE_2D, gbuffer->getLightingTexture());

      glUniform1f(SH_UN_LIGHTNESS, lightness);
    }
    static int SH_IN_VPOS;
    static int SH_UN_CAMERA;
    static int SH_UN_MVP;
    static int SH_UN_CAMERA_POS;
    static int SH_UN_TIME;
    static int SH_UN_POSTEX;
    static int SH_UN_NORMALTEX;
    static int SH_UN_ALBEDOTEX;
    static int SH_UN_SKYBOXTEX;
    static int SH_UN_SCREENSIZE;
    static int SH_UN_LIGHTINGTEX;
    static int SH_UN_LIGHTNESS;
};

int LightingSkyboxShader::SH_IN_VPOS = 0;
int LightingSkyboxShader::SH_UN_CAMERA = 0;
int LightingSkyboxShader::SH_UN_MVP = 1;
int LightingSkyboxShader::SH_UN_CAMERA_POS = 2;
int LightingSkyboxShader::SH_UN_TIME = 3;
int LightingSkyboxShader::SH_UN_POSTEX = 4;
int LightingSkyboxShader::SH_UN_NORMALTEX= 5;
int LightingSkyboxShader::SH_UN_ALBEDOTEX= 6;
int LightingSkyboxShader::SH_UN_LIGHTINGTEX = 7;
int LightingSkyboxShader::SH_UN_SCREENSIZE= 9;
int LightingSkyboxShader::SH_UN_SKYBOXTEX= 12;
int LightingSkyboxShader::SH_UN_LIGHTNESS = 13;

