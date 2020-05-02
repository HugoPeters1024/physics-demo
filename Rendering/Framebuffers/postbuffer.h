class Postbuffer {
    GLuint m_id, m_tex_output, m_depth_object;
    Logger m_logger = Logger("Postbuffer");
    int width = 1920;
    int height = 1080;
public:
    Postbuffer() {
      m_logger.logInfo("Creating the postbuffer");

      glGenFramebuffers(1, &m_id);
      glBindFramebuffer(GL_FRAMEBUFFER, m_id);

      glGenRenderbuffers(1, &m_depth_object);
      glBindRenderbuffer(GL_RENDERBUFFER, m_depth_object);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_object);

      glGenTextures(1, &m_tex_output);
      glBindTexture(GL_TEXTURE_2D, m_tex_output);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_tex_output, 0);

      GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
      glDrawBuffers(1, DrawBuffers);

      auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        m_logger.logError("Framebuffer not complete, error code: %i", fboStatus);

      m_logger.logInfo("Postbuffer successfully created");
    }

    void use()
    {
      glBindFramebuffer(GL_FRAMEBUFFER, m_id);
      glViewport(0, 0, width, height);
    }

    inline GLuint getFramebufferId() const { return m_id; }
    inline GLuint getOutputTexture() const { return m_tex_output; }
};