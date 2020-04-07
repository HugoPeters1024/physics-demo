#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H
#include <stdio.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <unordered_set>

#include "vec.h"


struct vertex {
    std::vector<float> v;
    void normalize() {
        float magnitude = 0.0f;
        for (int i = 0; i < v.size(); i++)
            magnitude += pow(v[i], 2.0f);
        magnitude = sqrt(magnitude);
        for (int i = 0; i < v.size(); i++)
            v[i] /= magnitude;
    }
    vertex operator-(vertex v2) {
        vertex v3;
        if (v.size() != v2.v.size()) {
            v3.v.push_back(0.0f);
            v3.v.push_back(0.0f);
            v3.v.push_back(0.0f);
        } else {
            for (int i = 0; i < v.size(); i++)
                v3.v.push_back(v[i] - v2.v[i]);
        }
        return v3;
    }
    vertex cross(vertex v2) {
        vertex v3;
        if (v.size() != 3 || v2.v.size() != 3) {
            v3.v.push_back(0.0f);
            v3.v.push_back(0.0f);
            v3.v.push_back(0.0f);
        } else {
            v3.v.push_back(v[1]*v2.v[2]-v[2]*v2.v[1]);
            v3.v.push_back(v[2]*v2.v[0]-v[0]*v2.v[2]);
            v3.v.push_back(v[0]*v2.v[1]-v[1]*v2.v[0]);
        }
        return v3;
    }
};

struct face {
    std::vector<int> vertex;
    std::vector<int> texture;
    std::vector<int> normal;
};

class cObj {
  private:
    std::vector<vertex> vertices;
    std::vector<vertex> texcoords;
    std::vector<vertex> normals;
    std::vector<vertex> parameters;
    std::vector<face> faces;
  public:
    cObj(std::string filename);
    ~cObj() {};
    void renderVertexBufferWithNormalsBytes(std::vector<float> &vbo, std::vector<GLubyte> &indices);
    void renderVertexBufferWithNormalsInts(std::vector<float> &vbo, std::vector<uint> &indices);
};

cObj::cObj(std::string filename) {
    std::ifstream ifs(filename.c_str(), std::ifstream::in);
    std::string line, key;
    while (ifs.good() && !ifs.eof() && std::getline(ifs, line)) {
        key = "";
        std::stringstream stringstream(line);
        stringstream >> key >> std::ws;


        if (key == "v") { // vertex
            vertex v; float x;
            while (!stringstream.eof()) {
                stringstream >> x >> std::ws;
                v.v.push_back(x);
            }
            vertices.push_back(v);
        } else if (key == "vp") { // parameter
            vertex v; float x;
            while (!stringstream.eof()) {
                stringstream >> x >> std::ws;
                v.v.push_back(x);
            }
            parameters.push_back(v);
        } else if (key == "vt") { // texture coordinate
            vertex v; float x;
            while (!stringstream.eof()) {
                stringstream >> x >> std::ws;
                v.v.push_back(x);
            }
            texcoords.push_back(v);
        } else if (key == "vn") { // normal
            vertex v; float x;
            while (!stringstream.eof()) {
                stringstream >> x >> std::ws;
                v.v.push_back(x);
            }
            v.normalize();
            normals.push_back(v);
        } else if (key == "f") { // face
            face f; int v, t, n;
            while (!stringstream.eof()) {
                stringstream >> v >> std::ws;
                f.vertex.push_back(v-1);
                if (stringstream.peek() == '/') {
                    stringstream.get();
                    if (stringstream.peek() == '/') {
                        stringstream.get();
                        stringstream >> n >> std::ws;
                        f.normal.push_back(n-1);
                    } else {
                        stringstream >> t >> std::ws;
                        f.texture.push_back(t-1);
                        if (stringstream.peek() == '/') {
                            stringstream.get();
                            stringstream >> n >> std::ws;
                            f.normal.push_back(n-1);
                        }
                    }
                }
            }
            faces.push_back(f);
        } else {

        }
    }
    ifs.close();
    std::cout << "               Name: " << filename << std::endl;
    std::cout << "           Vertices: " << vertices.size() << std::endl;
    std::cout << "         Parameters: " << parameters.size() << std::endl;
    std::cout << "Texture Coordinates: " << texcoords.size() << std::endl;
    std::cout << "            Normals: " << normals.size() << std::endl;
    std::cout << "              Faces: " << faces.size() << std::endl << std::endl;
}

struct float6 {
    float x,y,z,nx,ny,nz;
};

inline bool operator ==(const float6& lhs, const float6& rhs) {
  return lhs.x == rhs.x &&
         lhs.y == rhs.y &&
         lhs.z == rhs.z &&
         lhs.nx == rhs.nx &&
         lhs.ny == rhs.ny &&
         lhs.nz == rhs.nz;
};

struct hashfn {
    std::size_t operator() (const float6 &vertex) const
    {
      std::size_t hx = std::hash<float>()(vertex.x);
      std::size_t hy = std::hash<float>()(vertex.y);
      std::size_t hz = std::hash<float>()(vertex.z);
      std::size_t hnx = std::hash<float>()(vertex.nx);
      std::size_t hny = std::hash<float>()(vertex.ny);
      std::size_t hnz = std::hash<float>()(vertex.nz);
      return hx ^ hy ^ hz ^ hnx ^ hny ^ hnz;
    }
};

void cObj::renderVertexBufferWithNormalsInts(std::vector<float> &vbo, std::vector<uint> &indices) {
  vbo.clear();
  indices.clear();
  std::unordered_map<float6, GLuint, hashfn> visited;
  GLuint q=0;
  for(int i=0; i<faces.size(); i++)
  {
    for(int j=0; j<3; j++)
    {
      auto vIndex = faces[i].vertex[j];
      auto nIndex = faces[i].normal[j];
      float6 data = {
              vertices[vIndex].v[0],
              vertices[vIndex].v[1],
              vertices[vIndex].v[2],
              normals[nIndex].v[0],
              normals[nIndex].v[1],
              normals[nIndex].v[2],
      };
      auto tryInsert = visited.insert(std::pair<float6, GLuint>(data, q));
      if (tryInsert.second)
      {
        // New vertex, add it, refer to it.
        vbo.push_back(tryInsert.first->first.x);
        vbo.push_back(tryInsert.first->first.y);
        vbo.push_back(tryInsert.first->first.z);
        vbo.push_back(tryInsert.first->first.nx);
        vbo.push_back(tryInsert.first->first.ny);
        vbo.push_back(tryInsert.first->first.nz);
        indices.push_back(q);
        q++;
        if (q==0) {
          g_logError("Indices of model do not fit in an uint, what?");
          exit(-6);
        }
      }
      else{
        // Existing vertex, only refer to it.
        indices.push_back(tryInsert.first->second);
      }
    }
  }

  g_logDebug("Converted vbo contains %u entries and has %u indices referring", vbo.size()/6, indices.size());
}


#endif
