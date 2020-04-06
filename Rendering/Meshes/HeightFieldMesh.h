class HeightFieldMesh
{
private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_vertex_count;
    const DefaultShader* m_shader;
    static Vector3 normalAverage(std::vector<Vector3> input)
    {
      Vector3 acc(0);
      for(auto &v : input)
      {
        acc += v;
      }
      return acc / input.size();
    }
public:
    HeightFieldMesh(float* data, int width, int height, int thickness, const DefaultShader* shader)
    {
      m_shader = shader;
      glGenVertexArrays(1, &m_vao);
      glBindVertexArray(m_vao);

      std::unordered_map<int, std::vector<Vector3>> normalAcc;
      std::unordered_map<int, Vector3> vertices;
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
      for(int y=0; y<height-1; y++) {
        for (int x = 0; x < width - 1; x++) {
          int x0 = x, y0 = y;
          int x1 = x + 1, y1 = y;
          int x2 = x + 1, y2 = y + 1;
          int x3 = x, y3 = y + 1;

          auto v0 = vertices[x0 + y0 * width];
          auto v1 = vertices[x1 + y1 * width];
          auto v2 = vertices[x2 + y2 * width];
          auto v3 = vertices[x3 + y3 * width];

          Vector3 n0 = normalAverage(normalAcc[x0 + y0 * width]);
          Vector3 n1 = normalAverage(normalAcc[x1 + y1 * width]);
          Vector3 n2 = normalAverage(normalAcc[x2 + y2 * width]);
          Vector3 n3 = normalAverage(normalAcc[x3 + y3 * width]);

          vbo_data.push_back(v0.x);
          vbo_data.push_back(v0.y);
          vbo_data.push_back(v0.z);
          vbo_data.push_back(n0.x);
          vbo_data.push_back(n0.y);
          vbo_data.push_back(n0.z);

          vbo_data.push_back(v1.x);
          vbo_data.push_back(v1.y);
          vbo_data.push_back(v1.z);
          vbo_data.push_back(n1.x);
          vbo_data.push_back(n1.y);
          vbo_data.push_back(n1.z);

          vbo_data.push_back(v3.x);
          vbo_data.push_back(v3.y);
          vbo_data.push_back(v3.z);
          vbo_data.push_back(n3.x);
          vbo_data.push_back(n3.y);
          vbo_data.push_back(n3.z);

          vbo_data.push_back(v1.x);
          vbo_data.push_back(v1.y);
          vbo_data.push_back(v1.z);
          vbo_data.push_back(n1.x);
          vbo_data.push_back(n1.y);
          vbo_data.push_back(n1.z);

          vbo_data.push_back(v2.x);
          vbo_data.push_back(v2.y);
          vbo_data.push_back(v2.z);
          vbo_data.push_back(n2.x);
          vbo_data.push_back(n2.y);
          vbo_data.push_back(n2.z);

          vbo_data.push_back(v3.x);
          vbo_data.push_back(v3.y);
          vbo_data.push_back(v3.z);
          vbo_data.push_back(n3.x);
          vbo_data.push_back(n3.y);
          vbo_data.push_back(n3.z);
        }
      }

      m_vertex_count = vbo_data.size() / 6;

      glGenBuffers(1, &m_vbo);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, width * height * 6 * 6 * sizeof(float), vbo_data.data(), GL_STATIC_DRAW);

      glEnableVertexAttribArray(DefaultShader::SH_IN_VPOS);
      glEnableVertexAttribArray(DefaultShader::SH_IN_VNORMAL);
      glVertexAttribPointer(DefaultShader::SH_IN_VPOS, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(sizeof(float) * 0));
      glVertexAttribPointer(DefaultShader::SH_IN_VNORMAL, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(sizeof(float) * 3));
      glBindVertexArray(0);

    }
    void draw(const Camera::Camera* camera, const Matrix4& mvp) const {
      glBindVertexArray(m_vao);
      m_shader->use(camera, mvp);
      glDrawArrays(GL_TRIANGLES, 0, m_vertex_count);
      glBindVertexArray(0);
    }
};

