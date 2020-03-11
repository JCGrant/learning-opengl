#pragma once

#include <assimp/scene.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "mesh.h"
#include "shader.h"

#include <vector>

using namespace std;

unsigned int loadTexture(string path);

unsigned int loadCubemap(vector<std::string> faces);

class Model {
public:
  vector<Texture> textures_loaded;

  vector<Mesh> meshes;
  string directory;
  bool gammaCorrection;

  Model(string const &path, bool gamma = false);

  void Draw(Shader shader);

private:
  // loads a model with supported ASSIMP extensions from file and stores the
  // resulting meshes in the meshes vector.
  void loadModel(string const &path);

  // processes a node in a recursive fashion. Processes each individual mesh
  // located at the node and repeats this process on its children nodes (if
  // any).
  void processNode(aiNode *node, const aiScene *scene);

  Mesh processMesh(aiMesh *mesh, const aiScene *scene);

  // checks all material textures of a given type and loads the textures if
  // they're not loaded yet. The required info is returned as a Texture struct.
  vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                       string typeName);
};
