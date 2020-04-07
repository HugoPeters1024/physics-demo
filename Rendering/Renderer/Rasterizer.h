class Rasterizer : public GLFWRenderer
{
private:
    Logger m_logger = Logger("Rasterizer");
    std::shared_ptr<ResourceRepo> resourceRepo;
    std::vector<ISceneObject*> m_scene;
public:
    Rasterizer();
    void loop(const Camera::Camera* camera) override;
    inline GLFWwindow* getWindowPointer() const override { return m_window; }
    inline void addCube(rp3d::ProxyShape* shape, rp3d::BoxShape* box) override { m_scene.push_back(new CubeObject(resourceRepo.get(), shape, box)); }
    inline void addSphere(rp3d::ProxyShape* shape, rp3d::SphereShape* sphere) override { m_scene.push_back(new SphereObject(resourceRepo.get(), shape, sphere)); }
    inline void addHeightMap(rp3d::ProxyShape* shape, rp3d::HeightFieldShape* map, float* data) override { m_scene.push_back(new HeightFieldObject(resourceRepo.get(), shape, map, data)); }
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
  glEnable(GL_CULL_FACE);

  glfwSwapInterval(1);

  resourceRepo = std::make_shared<ResourceRepo>();

  m_logger.logDebug("Initialized");
}

void Rasterizer::loop(const Camera::Camera* camera) {
  glfwGetFramebufferSize(m_window, &m_window_width, &m_window_height);
  glViewport(0, 0, m_window_width, m_window_height);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (auto &obj : m_scene) {
    obj->draw(camera);
  }
  glfwSwapBuffers(m_window);
  glfwPollEvents();
}
