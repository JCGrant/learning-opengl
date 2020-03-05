#include <cmath>
#include <iostream>

#include <glad/glad.h>
// prevent clang-format reordering
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include "shader.h"

const unsigned int DEFAULT_WIDTH = 800;
const unsigned int DEFAULT_HEIGHT = 600;

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT,
                                        "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glEnable(GL_DEPTH_TEST);

  Shader shader("./shaders/vert_shader.glsl", "./shaders/frag_shader.glsl");

  float vertices[] = {
      // positions         // textures coords
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //
      0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, //
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, //
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, //
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, //
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, //
      -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, //
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //
      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, //

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, //
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, //
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, //
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
      0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, //
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, //
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, //
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, //
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, //
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f  //
  };
  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // texture coords attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  unsigned int texture1, texture2;

  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("resources/textures/container.jpg", &width,
                                  &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load("resources/textures/awesomeface.png", &width, &height,
                   &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  shader.use();
  shader.setInt("texture1", 0);
  shader.setInt("texture2", 1);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glm::mat4 model = glm::mat4(1.0f);
    model =
        glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.1f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f), (float)DEFAULT_WIDTH / (float)DEFAULT_HEIGHT, 0.1f,
        100.0f);

    shader.use();
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return 0;
}
