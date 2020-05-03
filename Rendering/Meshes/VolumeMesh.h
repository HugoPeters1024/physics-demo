class VolumeMesh
{
private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_veb;
    GLuint m_index_count;
    const LightingShader* m_shader;

public:
    VolumeMesh(const LightingShader* shader) {
      m_shader = shader;
      glGenVertexArrays(1, &m_vao);
      glBindVertexArray(m_vao);

      auto obj = cObj("Models/sphere.obj", 1.0 / 18.0);
      obj.quadsToTriangles();

      std::vector<float> vertices;
      std::vector<GLuint> indices;

      obj.renderVertexBufferPosOnly(vertices, indices);

      m_index_count = indices.size();

      glGenBuffers(1, &m_vbo);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

      glGenBuffers(1, &m_veb);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_veb);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

      glEnableVertexAttribArray(m_shader->SH_IN_VPOS);
      glVertexAttribPointer(m_shader->SH_IN_VPOS, 3, GL_FLOAT, GL_FALSE, 0*sizeof(float), (void*)(sizeof(float) * 0));
      glVertexArrayElementBuffer(m_vao, m_veb);
      glBindVertexArray(0);
    }
    void prepare(const Vector2 &screenSize, const Camera::Camera* camera, const GBuffer* gbuffer, GLuint skyboxTex, float lightness) const {
      m_shader->prepare(screenSize, camera, gbuffer, skyboxTex, lightness);
      glBindVertexArray(m_vao);
    }

    void draw(Light* light) const {
      m_shader->use(light, light->getMvp());
      glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, (void*)nullptr);
    }
};
