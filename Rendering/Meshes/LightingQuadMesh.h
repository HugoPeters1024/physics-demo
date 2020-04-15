class LightingQuadMesh
{
private:
    GLuint m_vao, m_vbo, m_veb;
    const LightingShader* m_shader;
public:
    LightingQuadMesh(const LightingShader* shader)
    {
      m_shader = shader;
      glGenVertexArrays(1, &m_vao);
      glBindVertexArray(m_vao);

      float quad_data[12] = {
              -1.f, -1.f, 0,
              1.f, -1.f, 0,
              1.f, 1.f, 0,
              -1.f, 1.f, 0,
      };

      GLubyte indices[6] = {
              0, 1, 2,
              0, 2, 3,
      };

      glGenBuffers(1, &m_vbo);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data), quad_data, GL_STATIC_DRAW);


      glGenBuffers(1, &m_veb);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_veb);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

      glEnableVertexAttribArray(QuadShader::SH_IN_VPOS);
      glVertexAttribPointer(QuadShader::SH_IN_VPOS, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 0));
      glVertexArrayElementBuffer(m_vao, m_veb);

      glBindVertexArray(0);
    }

    void draw(const Vector2 &screenSize, const Light* light, const Camera::Camera* camera, const GBuffer* gbuffer) const {
      glBindVertexArray(m_vao);
      m_shader->use(screenSize, light, camera, Matrix4::Identity(), gbuffer);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)nullptr);
      glBindVertexArray(0);
    }

};
