class HaloMesh
{
private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_veb;
    GLuint m_index_count;
    const HaloShader* m_shader;

public:
    HaloMesh(const HaloShader* shader) {
      m_shader = shader;
      glGenVertexArrays(1, &m_vao);
      glBindVertexArray(m_vao);

      auto obj = cObj("Models/circle.obj", 40.0);
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

    void prepare(const Camera::Camera* camera) const {
      m_shader->prepare(camera);
      glBindVertexArray(m_vao);
    }

    void draw(Light* light, const Camera::Camera* camera) const {
      Vector3 viewDir = (camera->getPosition() - light->position).normalize();
      Vector3 xz = Vector3(viewDir.x, 0, viewDir.z).normalize();

      float theta = -atan2(xz.x, -xz.z);
      if (theta < 0)
        theta = theta + 2 * 3.1415926f;
      float phi = atan2(viewDir.y, sqrt(viewDir.x * viewDir.x + viewDir.z * viewDir.z));

      Matrix4 rotation = Matrix4::FromAxisRotations(-phi, -theta, 0);
      Matrix4 translation = Matrix4::FromTranslation(light->position);
      Matrix4 scale = Matrix4::FromScale(light->radius());
      m_shader->use(light, translation * scale * rotation);
      glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, (void*)nullptr);
    }
};
