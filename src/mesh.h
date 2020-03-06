#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "shader.h"

using namespace std;

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
  glm::vec3 tangent;
  glm::vec3 bitangent;
};

struct Texture {
  unsigned int id;
  string type;
  string path;
};

class Mesh {
public:
  vector<Vertex> vertices;
  vector<unsigned int> indices;
  vector<Texture> textures;
  unsigned int VAO;

  Mesh(vector<Vertex> vertices, vector<unsigned int> indices,
       vector<Texture> textures);

  void Draw(Shader shader);

private:
  unsigned int VBO, EBO;
  void setupMesh();
};
