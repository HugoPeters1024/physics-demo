class Rasterizer : public GLFWRenderer
{
private:
    Logger m_logger = Logger("Rasterizer");
    GLuint m_shader;
    std::shared_ptr<MeshRepo> meshRepo;
    std::vector<ISceneObject*> m_scene;
public:
    Rasterizer();
    void loop(const Camera::Camera& camera) override;
    inline GLFWwindow* getWindowPointer() const override { return m_window; }
    inline void addCube(rp3d::ProxyShape* shape, rp3d::BoxShape* box) override { m_scene.push_back(new CubeObject(meshRepo.get(), shape, box)); }
    inline void addHeightMap(rp3d::ProxyShape* shape, rp3d::HeightFieldShape* map, float* data) override { m_scene.push_back(new HeightFieldObject(shape, map, data)); }
};

Rasterizer::Rasterizer() {
  if (!glfwInit())
  {
    m_logger.logError("Could not initialize GLFW");
    exit(-2);
  }

  glfwSetErrorCallback(error_callback);
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(GLDEBUGPROC(gl_debug_output), nullptr);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  m_window = glfwCreateWindow(604, 480, "Physics", NULL, NULL);
  if (!m_window){
    m_logger.logError("Could not create window.");
    exit(-3);
  }
  glfwMakeContextCurrent(m_window);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDisable(GL_CULL_FACE);

  glfwSwapInterval(1);

  meshRepo = std::make_shared<MeshRepo>();
  auto vs = CompileShader(GL_VERTEX_SHADER, ShaderSources::vs_src);
  auto fs = CompileShader(GL_FRAGMENT_SHADER, ShaderSources::fs_src);
  m_shader = GenerateProgram(vs, fs);

  m_logger.logDebug("Initialized");
}

void Rasterizer::loop(const Camera::Camera& camera) {
  glfwGetFramebufferSize(m_window, &m_window_width, &m_window_height);
  glViewport(0, 0, m_window_width, m_window_height);

  glUseProgram(m_shader);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mat4x4 cam_matrix;
  camera.getMatrix().unpack(cam_matrix);

  Vector3 camPos = camera.getPosition();
  glUniformMatrix4fv(SH_UNIFORM_CAMERA, 1, GL_FALSE, (const GLfloat*)(cam_matrix));
  glUniform3f(SH_UNIFORM_CAMERA_POS, camPos.x, camPos.y, camPos.z);
  glUniform1f(SH_UNIFORM_TIME, glfwGetTime());

  for (auto &obj : m_scene) {
    obj->draw();
  }
  glfwSwapBuffers(m_window);
  glfwPollEvents();
}
