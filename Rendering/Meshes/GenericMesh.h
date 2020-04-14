class GenericMesh
{
private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_veb;
    GLuint m_index_count;
    const GBufferShader* m_shader;

public:
    GenericMesh(const char* modelFile, const GBufferShader* shader) {
      m_shader = shader;
      glGenVertexArrays(1, &m_vao);
      glBindVertexArray(m_vao);

      auto obj = cObj(modelFile);

      //float vertices[obj.vertexCount()*6];
      //GLubyte indices[obj.faceCount()*3];

      std::vector<float> vertices;
      std::vector<GLuint> indices;

      obj.renderVertexBufferWithNormalsInts(vertices, indices);
      m_index_count = indices.size();

      glGenBuffers(1, &m_vbo);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

      glGenBuffers(1, &m_veb);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_veb);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

      glEnableVertexAttribArray(m_shader->SH_IN_VPOS);
      glEnableVertexAttribArray(m_shader->SH_IN_VNORMAL);
      glVertexAttribPointer(m_shader->SH_IN_VPOS, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(sizeof(float) * 0));
      glVertexAttribPointer(m_shader->SH_IN_VNORMAL, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(sizeof(float) * 3));
      glVertexArrayElementBuffer(m_vao, m_veb);
      glBindVertexArray(0);
    }

    void draw(const Camera::Camera* camera, const Matrix4& mvp, GLuint texture) const {
      glBindVertexArray(m_vao);
      m_shader->use(camera, mvp, texture);
      glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, (void*)nullptr);
      glBindVertexArray(0);
    }
};
