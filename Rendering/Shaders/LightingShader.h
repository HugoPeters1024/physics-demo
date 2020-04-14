static const char* lighting_vs_src = R"(
      #version 450

      layout(location = 0) in vec3 vPos;

      out vec2 uv;

      void main()
      {
        gl_Position = vec4(vPos, 1);
        uv = (vPos.xy + 1) / 2;
      }
    )";

static const char* lighting_fs_src = R"(
      #version 450

      layout(location = 2) uniform vec3 uCamPos;
      layout(location = 3) uniform float uTime;

      layout(location = 4) uniform sampler2D posTex;
      layout(location = 5) uniform sampler2D normalTex;
      layout(location = 6) uniform sampler2D albedoTex;

      in vec2 uv;

      out vec3 color;

      void main()
      {
          vec3 fragPos = texture(posTex, uv).xyz;
          vec3 fragNormal = texture(normalTex, uv).xyz;
          // Fragment is outside the world
          if (dot(fragNormal, fragNormal) < 0.98) {
              color = vec3(0);
              return;
          }
          vec3 fragColor = texture(albedoTex, uv).xyz;

          vec3 lightPos = vec3(10*sin(uTime), 20, 10*cos(uTime));
          vec3 lightCol = vec3(1) * 500;

          vec3 lightRay = lightPos - fragPos;
          float lightDis2 = dot(lightRay, lightRay);
          float falloff = 1.0f / lightDis2;
          vec3 lightNormal = lightRay / sqrt(lightDis2);
          float diffuse = max(dot(lightNormal, fragNormal), 0);

          vec3 eye = normalize(fragPos - uCamPos);
          vec3 refl = normalize(reflect(eye, fragNormal));
          float spec = pow(max(dot(refl, lightNormal), 0), 30) * 0.6f;

          float ambient = 0.1f;
          color = ambient * fragColor + (diffuse + spec) * fragColor * lightCol * falloff;
      }
    )";

class LightingShader {
private:
    GLuint m_program;
public:
    LightingShader() {
      GLuint vs = CompileShader(GL_VERTEX_SHADER, lighting_vs_src);
      GLuint fs = CompileShader(GL_FRAGMENT_SHADER, lighting_fs_src);
      m_program = GenerateProgram(vs, fs);
    }
    void use(const Camera::Camera* camera, const GBuffer* gbuffer)const {
      glUseProgram(m_program);

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
    }
    static int SH_UN_CAMERA_POS;
    static int SH_UN_TIME;
    static int SH_UN_POSTEX;
    static int SH_UN_NORMALTEX;
    static int SH_UN_ALBEDOTEX;
};

int LightingShader::SH_UN_CAMERA_POS = 2;
int LightingShader::SH_UN_TIME = 3;
int LightingShader::SH_UN_POSTEX = 4;
int LightingShader::SH_UN_NORMALTEX= 5;
int LightingShader::SH_UN_ALBEDOTEX= 6;

