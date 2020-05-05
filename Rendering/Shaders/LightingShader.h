static const char* lighting_vs_src = R"(
      #version 450

      layout(location = 0) in vec3 vPos;

      layout(location = 0) uniform mat4 uCamera;
      layout(location = 1) uniform mat4 uMvp;

      out vec3 volumeWorldPos;

      void main()
      {
        vec4 worldPos = uMvp * vec4(vPos, 1);
        gl_Position = uCamera * worldPos;
        volumeWorldPos = worldPos.xyz;
      }
    )";

static const char* lighting_fs_src = R"(
      #version 450

      layout(location = 2) uniform vec3 uCamPos;
      layout(location = 3) uniform float uTime;

      layout(location = 4) uniform sampler2D posTex;
      layout(location = 5) uniform sampler2D normalTex;
      layout(location = 6) uniform sampler2D albedoTex;
      layout(location = 7) uniform sampler2D lightingTex;
      layout(location = 8) uniform vec3 lightProperties;

      layout(location = 9) uniform vec2 screenCorrection;
      layout(location = 10) uniform vec3 lightPos;
      layout(location = 11) uniform vec3 lightCol;
      layout(location = 13) uniform float lightness;


      in vec3 volumeWorldPos;
      out vec4 color;

      void main()
      {
          //color = vec4(1,1,1,0.2f); return;
          vec2 uv = gl_FragCoord.xy * screenCorrection;

          vec3 fragNormal = normalize(texture(normalTex, uv).xyz);
          vec3 fragPos = texture(posTex, uv).xyz;
          vec3 fragColor = texture(albedoTex, uv).xyz;

          float falloff_const_term = lightProperties.x;
          float falloff_linear_term = lightProperties.y;
          float falloff_quadratic_term = lightProperties.z;

          vec3 lightRay = lightPos - fragPos;
          float lightDis2 = dot(lightRay, lightRay);
          float lightDis = sqrt(lightDis2);


          float falloff = 1.0f / (falloff_const_term + falloff_linear_term * lightDis + falloff_quadratic_term * lightDis2);


          vec3 lightNormal = lightRay / lightDis;
          float diffuse = max(dot(lightNormal, fragNormal), 0);

          vec3 lightingValues = texture(lightingTex, uv).rgb;

          vec3 eye = normalize(fragPos - uCamPos);
          vec3 refl = reflect(eye, fragNormal);
          float spec = pow(max(dot(refl, lightNormal), 0), 30) * lightingValues.x;

          float albedoDamp = max(1 - lightingValues.y - lightingValues.z, 0);

          color = vec4((diffuse * albedoDamp + spec) * fragColor * lightCol * falloff, 1);
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
    void prepare(const Vector2 screenSize, const Camera::Camera* camera, const GBuffer* gbuffer, float lightness) const {
      glUseProgram(m_program);
      glUniform2f(SH_UN_SCREENCORRECTION, 1.0f / screenSize.x, 1.0f / screenSize.y);

      mat4x4 e_camera;
      camera->getMatrix().unpack(e_camera);
      glUniformMatrix4fv(SH_UN_CAMERA, 1, GL_FALSE, (const GLfloat*)e_camera);

      Vector3 camPos = camera->getPosition();
      glUniform3f(SH_UN_CAMERA_POS, camPos.x, camPos.y, camPos.z);

      glUniform1i(SH_UN_POSTEX, 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, gbuffer->getPositionTexture());

      glUniform1i(SH_UN_NORMALTEX, 1);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, gbuffer->getNormalTexture());

      glUniform1i(SH_UN_ALBEDOTEX, 2);
      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_2D, gbuffer->getAlbedoTexture());

      glUniform1i(SH_UN_LIGHTINGTEX, 4);
      glActiveTexture(GL_TEXTURE4);
      glBindTexture(GL_TEXTURE_2D, gbuffer->getLightingTexture());

      glUniform1f(SH_UN_LIGHTNESS, lightness);
      glUniform1f(SH_UN_TIME, glfwGetTime());
    }
    void use(const Light* light, const Matrix4& mvp) const {

      mat4x4 e_mvp;
      mvp.unpack(e_mvp);
      glUniformMatrix4fv(SH_UN_MVP, 1, GL_FALSE, (const GLfloat*)e_mvp);

      glUniform3f(SH_UN_LIGHTINGPROPERTIES, light->constant, light->linear, light->quadratic);
      glUniform3f(SH_UN_LIGHTPOS, light->position.x, light->position.y, light->position.z);
      glUniform3f(SH_UN_LIGHTCOLOR, light->color.x, light->color.y, light->color.z);
    }
    static int SH_IN_VPOS;
    static int SH_UN_CAMERA;
    static int SH_UN_MVP;
    static int SH_UN_CAMERA_POS;
    static int SH_UN_TIME;
    static int SH_UN_POSTEX;
    static int SH_UN_NORMALTEX;
    static int SH_UN_ALBEDOTEX;
    static int SH_UN_LIGHTINGPROPERTIES;
    static int SH_UN_SCREENCORRECTION;
    static int SH_UN_LIGHTPOS;
    static int SH_UN_LIGHTCOLOR;
    static int SH_UN_LIGHTINGTEX;
    static int SH_UN_LIGHTNESS;
};

int LightingShader::SH_IN_VPOS = 0;
int LightingShader::SH_UN_CAMERA = 0;
int LightingShader::SH_UN_MVP = 1;
int LightingShader::SH_UN_CAMERA_POS = 2;
int LightingShader::SH_UN_TIME = 3;
int LightingShader::SH_UN_POSTEX = 4;
int LightingShader::SH_UN_NORMALTEX= 5;
int LightingShader::SH_UN_ALBEDOTEX= 6;
int LightingShader::SH_UN_LIGHTINGTEX = 7;
int LightingShader::SH_UN_LIGHTINGPROPERTIES = 8;
int LightingShader::SH_UN_SCREENCORRECTION= 9;
int LightingShader::SH_UN_LIGHTPOS= 10;
int LightingShader::SH_UN_LIGHTCOLOR= 11;
int LightingShader::SH_UN_LIGHTNESS = 13;

