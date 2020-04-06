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
