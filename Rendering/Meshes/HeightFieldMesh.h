class HeightFieldMesh
{
private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_veb;
    GLuint m_vertex_count;
    const GBufferShader* m_shader;
    Material m_material;
    static Vector3 normalAverage(std::vector<Vector3>& input)
    {
      Vector3 acc(0);
      for(auto &v : input)
      {
        acc += v;
      }
      acc.normalize();
      return acc;
    }
public:
    HeightFieldMesh(float* data, int width, int height, int thickness, const GBufferShader* shader)
    {
      m_shader = shader;
      m_material.diffuseColor = Vector3(1);
      m_material.specular = 0;
      glGenVertexArrays(1, &m_vao);
      glBindVertexArray(m_vao);

      std::vector<Vector3> normalAcc[width * height];
      Vector3 vertices[width * height];
      for(int y=0; y<height-1; y++) {
        for (int x = 0; x < width - 1; x++) {
          int x0 = x, y0 = y;
          int x1 = x + 1, y1 = y;
          int x2 = x + 1, y2 = y + 1;
          int x3 = x, y3 = y + 1;

          float scale = 1.0f;
          Vector3 v0(x0 * scale - width / 2.0f, data[x0 + y0 * width] - thickness / 2.0f, y0 * scale - height / 2.0f);
          Vector3 v1(x1 * scale - width / 2.0f, data[x1 + y1 * width] - thickness / 2.0f, y1 * scale - height / 2.0f);
          Vector3 v2(x2 * scale - width / 2.0f, data[x2 + y2 * width] - thickness / 2.0f, y2 * scale - height / 2.0f);
          Vector3 v3(x3 * scale - width / 2.0f, data[x3 + y3 * width] - thickness / 2.0f, y3 * scale - height / 2.0f);

          Vector3 n1(-Vector3::cross(v1 - v0, v3 - v0).normalize());
          Vector3 n2(-Vector3::cross(v1 - v0, v3 - v1).normalize());

          vertices[x0 + y0 * width] = v0;
          vertices[x1 + y1 * width] = v1;
          vertices[x3 + y3 * width] = v2;

          vertices[x1 + y1 * width] = v1;
          vertices[x2 + y2 * width] = v2;
          vertices[x3 + y3 * width] = v3;

          normalAcc[x0 + y0 * width].push_back(n1);
          normalAcc[x1 + y1 * width].push_back(n1);
          normalAcc[x3 + y3 * width].push_back(n1);

          normalAcc[x1 + y1 * width].push_back(n2);
          normalAcc[x2 + y2 * width].push_back(n2);
          normalAcc[x3 + y3 * width].push_back(n2);
        }
      }

      std::vector<float> vbo_data;
      for(int y=0; y<height; y++) {
        for (int x = 0; x < width; x++) {

          auto v = vertices[x + y * width];
          Vector3 n = normalAverage(normalAcc[x + y * width]);

          vbo_data.push_back(v.x);
          vbo_data.push_back(v.y);
          vbo_data.push_back(v.z);
          vbo_data.push_back(n.x);
          vbo_data.push_back(n.y);
          vbo_data.push_back(n.z);

          // Use x and z vertices again for uv coords.
          vbo_data.push_back(v.x / 20.0f);
          vbo_data.push_back(v.z / 20.0f);
        }
      }

      std::vector<uint> indices;
      for(int y=0; y<height-1; y++)
      {
        for(int x=0; x<width-1; x++) {
          int p0 = x + y * width;
          int p1 = (x+1) + y * width;
          int p2 = (x+1) + (y+1) * width;
          int p3 = x + (y+1) * width;

          indices.push_back(p2);
          indices.push_back(p1);
          indices.push_back(p0);

          indices.push_back(p0);
          indices.push_back(p3);
          indices.push_back(p2);
        }
      }
      m_vertex_count = indices.size();

      glGenBuffers(1, &m_veb);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_veb);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

      glGenBuffers(1, &m_vbo);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, vbo_data.size() * sizeof(float), vbo_data.data(), GL_STATIC_DRAW);

      glEnableVertexAttribArray(m_shader->SH_IN_VPOS);
      glEnableVertexAttribArray(m_shader->SH_IN_VNORMAL);
      glEnableVertexAttribArray(m_shader->SH_IN_VUV);
      glVertexAttribPointer(m_shader->SH_IN_VPOS, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(sizeof(float) * 0));
      glVertexAttribPointer(m_shader->SH_IN_VNORMAL, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(sizeof(float) * 3));
      glVertexAttribPointer(m_shader->SH_IN_VUV, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(sizeof(float) * 6));
      glVertexArrayElementBuffer(m_vao, m_veb);
      glBindVertexArray(0);

    }
    void draw(const Camera::Camera* camera, const Matrix4& mvp, GMaterial material) const {
      glBindVertexArray(m_vao);
      m_shader->use(mvp, material);
      glDrawElements(GL_TRIANGLES, m_vertex_count, GL_UNSIGNED_INT, (void*)nullptr);
      glBindVertexArray(0);
    }
};

