static const char* halo_vs_src = R"(
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

static const char* halo_fs_src = R"(
      #version 450

      layout(location = 2) uniform vec3 uCamPos;
      layout(location = 3) uniform float uTime;

      layout(location = 8) uniform vec3 lightProperties;

      layout(location = 10) uniform vec3 lightPos;
      layout(location = 11) uniform vec3 lightCol;


      in vec3 volumeWorldPos;
      out vec4 color;

      void main()
      {
          float maxCol = max(max(lightCol.x, lightCol.y), lightCol.z);
          vec3 normalizedColor = lightCol / maxCol;
          vec3 toCenter = (lightPos - volumeWorldPos) * 5;
          float dis2 = dot(toCenter, toCenter);

          vec3 toCamera = (uCamPos - volumeWorldPos) * 0.22f;
          float disToCamera2 = dot(toCamera, toCamera);
          color = vec4(vec3(lightCol/(dis2+disToCamera2+1))*0.05f,1);
      }
    )";

class HaloShader {
private:
    GLuint m_program;
public:
    HaloShader() {
      GLuint vs = CompileShader(GL_VERTEX_SHADER, halo_vs_src);
      GLuint fs = CompileShader(GL_FRAGMENT_SHADER, halo_fs_src);
      m_program = GenerateProgram(vs, fs);
    }
    void prepare(const Camera::Camera* camera) const {
      glUseProgram(m_program);

      mat4x4 e_camera;
      camera->getMatrix().unpack(e_camera);
      glUniformMatrix4fv(SH_UN_CAMERA, 1, GL_FALSE, (const GLfloat*)e_camera);

      Vector3 camPos = camera->getPosition();
      glUniform3f(SH_UN_CAMERA_POS, camPos.x, camPos.y, camPos.z);

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
    static int SH_UN_LIGHTINGPROPERTIES;
    static int SH_UN_LIGHTPOS;
    static int SH_UN_LIGHTCOLOR;
};

int HaloShader::SH_IN_VPOS = 0;
int HaloShader::SH_UN_CAMERA = 0;
int HaloShader::SH_UN_MVP = 1;
int HaloShader::SH_UN_CAMERA_POS = 2;
int HaloShader::SH_UN_TIME = 3;
int HaloShader::SH_UN_LIGHTINGPROPERTIES = 8;
int HaloShader::SH_UN_LIGHTPOS= 10;
int HaloShader::SH_UN_LIGHTCOLOR= 11;

