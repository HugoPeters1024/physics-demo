class Rasterizer : public GLFWRenderer
{
private:
    Logger m_logger = Logger("Rasterizer");
    std::shared_ptr<ResourceRepo> resourceRepo;
    std::shared_ptr<GBuffer> gbuffer;
    std::shared_ptr<Postbuffer> postbuffer;
    std::vector<ISceneObject*> m_scene;
    std::vector<Light*> m_lights;
public:
    Rasterizer();
    void loop(const Camera::Camera* camera) override;
    inline GLFWwindow* getWindowPointer() const override { return m_window; }
    inline void addCube(rp3d::ProxyShape* shape, rp3d::BoxShape* box) override { m_scene.push_back(new CubeObject(resourceRepo.get(), shape, box)); }
    inline void addSphere(rp3d::ProxyShape* shape, rp3d::SphereShape* sphere) override { m_scene.push_back(new SphereObject(resourceRepo.get(), shape, sphere)); }
    inline void addHeightMap(rp3d::ProxyShape* shape, rp3d::HeightFieldShape* map, float* data) override { m_scene.push_back(new HeightFieldObject(resourceRepo.get(), shape, map, data)); }
    inline void addLight(Light* light) override { m_lights.push_back(light); }
};

Rasterizer::Rasterizer() {
  if (!glfwInit())
  {
    m_logger.logError("Could not initialize GLFW");
    exit(-2);
  }

  glfwSetErrorCallback(error_callback);
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(GLDEBUGPROC(gl_debug_output), nullptr);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  m_window = glfwCreateWindow(1920, 1080, "Physics", glfwGetPrimaryMonitor(), NULL);
  if (!m_window){
    m_logger.logError("Could not create window.");
    exit(-3);
  }
  glfwMakeContextCurrent(m_window);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);
  glDisable(GL_FRAMEBUFFER_SRGB);

  glfwSwapInterval(1);

  gbuffer = std::make_shared<GBuffer>();
  postbuffer = std::make_shared<Postbuffer>();
  resourceRepo = std::make_shared<ResourceRepo>();

  m_logger.logDebug("Initialized");
}

void Rasterizer::loop(const Camera::Camera* camera) {
  float lightness = sin(glfwGetTime() / 10) * 0.5 + 0.5;

  gbuffer->use();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  for (auto &obj : m_scene) {
    obj->draw(camera);
  }

  glBindFramebuffer(GL_READ_FRAMEBUFFER, gbuffer->getFramebufferID());
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postbuffer->getFramebufferId());
  glBlitFramebuffer(0, 0, m_window_width, m_window_height, 0, 0, m_window_width, m_window_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

  postbuffer->use();
  glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glfwGetFramebufferSize(m_window, &m_window_width, &m_window_height);
  Vector2 screenSize = Vector2((float)m_window_width, (float)m_window_height);
  glViewport(0, 0, m_window_width, m_window_height);


  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);
  glCullFace(GL_FRONT);
  for(Light* light : m_lights) {
    resourceRepo->getVolumeMesh()->draw(
            screenSize,
            light,
            camera,
            gbuffer.get(),
            resourceRepo->getSkyBoxTexture(),
            lightness);
  }
  glCullFace(GL_BACK);
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  resourceRepo->getSkyboxMesh()->draw(
          camera,
          Matrix4::FromTranslation(camera->getPosition()) * Matrix4::FromScale(1000),
          resourceRepo->getSkyBoxTexture(),
          lightness);

  // Now draw to the screen using the post quad
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  resourceRepo->getPostQuadMesh()->draw(postbuffer->getOutputTexture());

  glfwPollEvents();
  glfwSwapBuffers(m_window);
};
