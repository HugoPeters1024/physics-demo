class GBuffer
{
private:
    GLuint m_id, m_tex_normal, m_tex_pos, m_tex_albedo, m_depth_object;
    Logger m_logger = Logger("GBuffer");
    int width = 1920;
    int height = 1080;
public:
    GBuffer() {
      m_logger.logDebug("Creating the GBuffer");
      glGenFramebuffers(1, &m_id);
      glBindFramebuffer(GL_FRAMEBUFFER, m_id);

      glGenRenderbuffers(1, &m_depth_object);
      glBindRenderbuffer(GL_RENDERBUFFER, m_depth_object);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_object);

      glGenTextures(1, &m_tex_normal);
      glBindTexture(GL_TEXTURE_2D, m_tex_normal);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_tex_normal, 0);

      glGenTextures(1, &m_tex_pos);
      glBindTexture(GL_TEXTURE_2D, m_tex_pos);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_tex_pos, 0);

      glGenTextures(1, &m_tex_albedo);
      glBindTexture(GL_TEXTURE_2D, m_tex_albedo);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, m_tex_albedo, 0);


      GLenum DrawBuffers[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
      glDrawBuffers(3, DrawBuffers);

      auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        m_logger.logError("Framebuffer not complete, error code: %i", fboStatus);

      m_logger.logInfo("GBuffer successfully created");
    }

    void use()
    {
      glBindFramebuffer(GL_FRAMEBUFFER, m_id);
      glViewport(0, 0, width, height);
    }

    GLuint getNormalTexture() const { return m_tex_normal; }
    GLuint getPositionTexture() const { return m_tex_pos; }
    GLuint getAlbedoTexture() const { return m_tex_albedo; }
};