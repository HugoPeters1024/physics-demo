class IMesh {
public:
    virtual void draw(Matrix4& mvp) const = 0;
};

class Triangle2D : public IMesh {
private:
    GLuint m_vao;
    GLuint m_vbo;
  public:
    Triangle2D() {
      glGenVertexArrays(1, &m_vao);
      glBindVertexArray(m_vao);

      float vertices[9]
      {
          -1, 1, 0,
          1, 1, 0,
          0.0f, -1, 0,
      };

      glGenBuffers(1, &m_vbo);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glEnableVertexAttribArray(SH_IN_VPOS);
      glVertexAttribPointer(SH_IN_VPOS, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 0));
      glBindVertexArray(0);
    }
    ~Triangle2D()  {
      glDeleteVertexArrays(1, &m_vao);
      glDeleteBuffers(1, &m_vbo);
    }
    void draw(Matrix4& mvp) const override {
      glBindVertexArray(m_vao);
      mat4x4 e_mvp;
      mvp.unpack(e_mvp);
      glUniformMatrix4fv(SH_UNIFORM_MVP, 1, GL_FALSE, (const GLfloat*)e_mvp);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glBindVertexArray(0);
    }
};

class Cube : public IMesh
{
private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_veb;
    GLuint m_index_count;
    Vector3 m_size;
public:
    Cube(float sx, float sy, float sz) {
      m_size = Vector3(sx, sy, sz)*2;
      glGenVertexArrays(1, &m_vao);
      glBindVertexArray(m_vao);

      auto obj = cObj("cube.obj");

      //float vertices[obj.vertexCount()*6];
      //GLubyte indices[obj.faceCount()*3];

      std::vector<float> vertices;
      std::vector<GLubyte> indices;

      obj.renderVertexBufferWithNormalsBytes(vertices, indices);
      m_index_count = indices.size();

      glGenBuffers(1, &m_vbo);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

      glGenBuffers(1, &m_veb);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_veb);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), indices.data(), GL_STATIC_DRAW);

      glEnableVertexAttribArray(SH_IN_VPOS);
      glEnableVertexAttribArray(SH_IN_VNORMAL);
      glVertexAttribPointer(SH_IN_VPOS, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(sizeof(float) * 0));
      glVertexAttribPointer(SH_IN_VNORMAL, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(sizeof(float) * 3));
      glVertexArrayElementBuffer(m_vao, m_veb);
      glBindVertexArray(0);
    }

    void draw(Matrix4& mvp) const override {
      mvp = mvp * Matrix4::FromScale(m_size);
      glBindVertexArray(m_vao);
      mat4x4 e_mvp;
      mvp.unpack(e_mvp);
      glUniformMatrix4fv(SH_UNIFORM_MVP, 1, GL_FALSE, (const GLfloat*)e_mvp);
      glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_BYTE, (void*)nullptr);
      glBindVertexArray(0);
    }
};
