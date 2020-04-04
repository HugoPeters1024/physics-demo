#include <memory>

void error_callback(int error, const char* description)
{
  fprintf(stderr, "Error: %s\n", description);
}

class Renderer
{
private:
    Logger m_logger = Logger("Renderer");
    GLFWwindow* m_window;
    int m_window_width;
    int m_window_height;
    GLuint m_shader;
    std::vector<std::unique_ptr<IMesh>> m_scene;
public:
    Renderer();
    ~Renderer() {
      m_logger.logDebug("Cleaning up...");
      glfwDestroyWindow(m_window);
      glfwTerminate();
    }
    void loop(const Matrix4 camera);
    inline bool shouldClose() { return glfwWindowShouldClose(m_window); }
    inline GLFWwindow* getWindowPointer() const { return m_window; }
    inline float getWindowRatio() const { return (float)m_window_width / (float)m_window_height; }
};

Renderer::Renderer() {
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

  auto vs = CompileShader(GL_VERTEX_SHADER, shaders::vs_src);
  auto fs = CompileShader(GL_FRAGMENT_SHADER, shaders::fs_src);
  m_shader = GenerateProgram(vs, fs);

  m_scene.push_back(std::make_unique<Cube>());

  m_logger.logDebug("Initialized");
}

void Renderer::loop(const Matrix4 camera) {
  glfwGetFramebufferSize(m_window, &m_window_width, &m_window_height);
  glViewport(0, 0, m_window_width, m_window_height);

  glUseProgram(m_shader);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mat4x4 cam_matrix;
  camera.unpack(cam_matrix);
  glUniformMatrix4fv(SH_UNIFORM_CAMERA, 1, GL_FALSE, (const GLfloat*)(cam_matrix));

  for (auto&& obj : m_scene) {
    obj->m_rot.y += 0.01f;
    obj->m_rot.x += 0.012f;
   // obj->m_pos.z += 0.01f;
    obj->draw();
  }
  glfwSwapBuffers(m_window);
  glfwPollEvents();
}
