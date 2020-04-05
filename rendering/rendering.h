void error_callback(int error, const char* description)
{
  fprintf(stderr, "Error: %s\n", description);
}

class IRenderer
{
public:
    virtual void loop(const Camera::Camera& camera) = 0;
    virtual GLFWwindow* getWindowPointer() const = 0;
    virtual bool shouldClose() = 0;
    virtual float getWindowRatio() const = 0;
    virtual void addCube(rp3d::ProxyShape* shape, rp3d::BoxShape* box) = 0;
};

class GLFWRenderer : public IRenderer
{
protected:
    GLFWwindow* m_window;
    int m_window_width;
    int m_window_height;
public:
    ~GLFWRenderer() {
      glfwDestroyWindow(m_window);
      glfwTerminate();
    }
    inline bool shouldClose() override { return glfwWindowShouldClose(m_window); }
    inline GLFWwindow* getWindowPointer() const override { return m_window; }
    inline float getWindowRatio() const override { return (float)m_window_width / (float)m_window_height; }
};

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
  auto vs = CompileShader(GL_VERTEX_SHADER, shaders::vs_src);
  auto fs = CompileShader(GL_FRAGMENT_SHADER, shaders::fs_src);
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
  glUniformMatrix4fv(SH_UNIFORM_CAMERA, 1, GL_FALSE, (const GLfloat*)(cam_matrix));

  for (auto &obj : m_scene) {
    obj->draw();
  }
  glfwSwapBuffers(m_window);
  glfwPollEvents();
}

class Raytracer : public GLFWRenderer
{
private:
    Logger m_logger = Logger("Raytracer");
    GLuint m_shader;
    std::shared_ptr<MeshRepo> meshRepo;
    std::vector<ISceneObject*> m_scene;
public:
    Raytracer();
    void loop(const Camera::Camera& camera) override;
    inline GLFWwindow* getWindowPointer() const override { return m_window; }
    inline void addCube(rp3d::ProxyShape* shape, rp3d::BoxShape* box) override { m_scene.push_back(new CubeObject(meshRepo.get(), shape, box)); }
};

Raytracer::Raytracer() {
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
  glfwSwapInterval(1);

  meshRepo = std::make_shared<MeshRepo>();
  auto vs = CompileShader(GL_VERTEX_SHADER, shaders::vs_src);
  auto fs = CompileShader(GL_FRAGMENT_SHADER, shaders::fs_src);
  m_shader = GenerateProgram(vs, fs);

  m_logger.logDebug("Initialized");
}

void Raytracer::loop(const Camera::Camera& camera) {
  glfwGetFramebufferSize(m_window, &m_window_width, &m_window_height);
  glViewport(0, 0, m_window_width, m_window_height);

  glUseProgram(m_shader);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glfwSwapBuffers(m_window);
  glfwPollEvents();
}
