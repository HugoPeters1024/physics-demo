class GMaterial {
public:
    GLuint m_diffuse;
    GLuint m_lighting;

    GMaterial() {}
    GMaterial(GLuint diffuse, GLuint lighting)
      : m_diffuse(diffuse), m_lighting(lighting) { }
};

