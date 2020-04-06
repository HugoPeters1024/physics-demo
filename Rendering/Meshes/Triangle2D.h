class Triangle2D {
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
    void draw(const Matrix4& mvp) const {
      glBindVertexArray(m_vao);
      mat4x4 e_mvp;
      mvp.unpack(e_mvp);
      glUniformMatrix4fv(SH_UNIFORM_MVP, 1, GL_FALSE, (const GLfloat*)e_mvp);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glBindVertexArray(0);
    }
};
