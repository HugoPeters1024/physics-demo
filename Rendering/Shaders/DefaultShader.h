static const char* vs_src = R"(
      #version 450

      layout(location = 0) in vec3 vPos;
      layout(location = 1) in vec3 vNormal;

      layout(location = 0) uniform mat4 uCamera;
      layout(location = 1) uniform mat4 uMvp;

      out vec3 fragPos;
      out vec3 fragNormal;

      void main()
      {
        vec4 worldPos = uMvp * vec4(vPos, 1.0);
        fragPos = worldPos.xyz;
        fragNormal = normalize((uMvp * vec4(vNormal,0)).xyz);
        gl_Position = uCamera * worldPos;
      }
    )";

static const char* fs_src = R"(
      #version 450

      layout(location = 2) uniform vec3 uCamPos;
      layout(location = 3) uniform float uTime;
      layout(location = 4) uniform vec3 uMaterialColor;
      layout(location = 5) uniform float uMaterialSpecular;

      in vec3 fragPos;
      in vec3 fragNormal;
      out vec3 color;

      void main()
      {
          vec3 lightPos = vec3(10*sin(uTime), 20, 10*cos(uTime));
          vec3 lightCol = vec3(1) * 500;

          vec3 lightRay = lightPos - fragPos;
          float lightDis2 = dot(lightRay, lightRay);
          float falloff = 1.0f / lightDis2;
          vec3 lightNormal = lightRay / sqrt(lightDis2);
          float diffuse = max(dot(lightNormal, fragNormal), 0);

          vec3 eye = normalize(fragPos - uCamPos);
          vec3 refl = normalize(reflect(eye, fragNormal));
          float spec = pow(max(dot(refl, lightNormal), 0), 30) * uMaterialSpecular;

          float ambient = 0.1f;
          color = ambient * uMaterialColor + (diffuse + spec) * uMaterialColor * lightCol * falloff;
      }
    )";

class DefaultShader {
private:
    GLuint m_program;
public:
    DefaultShader() {
      GLuint vs = CompileShader(GL_VERTEX_SHADER, vs_src);
      GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fs_src);
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

int DefaultShader::SH_IN_VPOS = 0;
int DefaultShader::SH_IN_VNORMAL = 1;

int DefaultShader::SH_UN_CAMERA = 0;
int DefaultShader::SH_UN_MVP = 1;
int DefaultShader::SH_UN_CAMERA_POS = 2;
int DefaultShader::SH_UN_TIME = 3;
int DefaultShader::SH_UN_MATERIAL_COLOR = 4;
int DefaultShader::SH_UN_MATERIAL_SPECULAR = 5;

