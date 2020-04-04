class IMesh {
public:
    Vector3 m_pos;
    Vector3 m_rot;
    virtual void draw() const = 0;
    virtual Matrix4 getMvp() const {
      return Matrix4::FromTranslation(m_pos) * Matrix4::FromAxisRotations(m_rot);
    }
};

class Triangle2D : public IMesh {
private:
    GLuint m_vao;
    GLuint m_vbo;
  public:
    Triangle2D() {
      m_pos = Vector3(0,0,0);
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
    void draw() const override {
      glBindVertexArray(m_vao);
      mat4x4 mvp;
      getMvp().unpack(mvp);
      glUniformMatrix4fv(SH_UNIFORM_MVP, 1, GL_FALSE, (const GLfloat*)(mvp));
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
public:
    Cube() {
      m_pos = Vector3(0,0,0);
      m_rot = Vector3(0,0,0);
      glGenVertexArrays(1, &m_vao);
      glBindVertexArray(m_vao);

      auto obj = cObj("cube.obj");

      float vertices[obj.vertexCount()*3];
      GLubyte indices[obj.faceCount()*3];

      obj.renderVertexBuffer(vertices);
      obj.renderIndexBufferBytes(indices);

      glGenBuffers(1, &m_vbo);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glGenBuffers(1, &m_veb);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_veb);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

      glEnableVertexAttribArray(SH_IN_VPOS);
      glVertexAttribPointer(SH_IN_VPOS, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 0));
      glVertexArrayElementBuffer(m_vao, m_veb);
      glBindVertexArray(0);
    }

    void draw() const override {
      glBindVertexArray(m_vao);
      mat4x4 mvp;
      getMvp().unpack(mvp);
      glUniformMatrix4fv(SH_UNIFORM_MVP, 1, GL_FALSE, (const GLfloat*)(mvp));
      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)nullptr);
      glBindVertexArray(0);
    }
};
