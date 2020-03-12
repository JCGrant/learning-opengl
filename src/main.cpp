#include <iostream>
#include <map>

#include <glad/glad.h>
// prevent clang-format reordering
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include "model.h"
#include "shader.h"

const unsigned int DEFAULT_WIDTH = 800;
const unsigned int DEFAULT_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = DEFAULT_WIDTH / 2.0;
float lastY = DEFAULT_HEIGHT / 2.0;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.processKeyboard(FORWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.processKeyboard(BACKWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.processKeyboard(LEFT, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.processKeyboard(RIGHT, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    camera.processKeyboard(UP, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    camera.processKeyboard(DOWN, deltaTime);
  }
}

void framebufferSizeCallback(__attribute__((unused)) GLFWwindow *window,
                             int width, int height) {
  glViewport(0, 0, width, height);
}

void mouseCallback(__attribute__((unused)) GLFWwindow *window, double xpos,
                   double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos; // reversed since y-coordinates go from bottom to top
  lastX = xpos;
  lastY = ypos;

  camera.processMouseMovement(xoffset, yoffset);
}

void scrollCallback(__attribute__((unused)) GLFWwindow *window,
                    __attribute__((unused)) double xoffset, double yoffset) {
  camera.processMouseScroll(yoffset);
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
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glEnable(GL_DEPTH_TEST);

  Shader shader("shaders/reflect.vert", "shaders/reflect.frag");
  Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");

  float cubeVertices[] = {
      // positions          // normals
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, //
      0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, //
      0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, //
      0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, //
      -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, //
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, //

      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, //
      0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, //
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //
      -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //
      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, //

      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f, //
      -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f, //
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f, //
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f, //
      -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f, //
      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f, //

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //
      0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f, //
      0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, //
      0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, //
      0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f, //
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //

      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, //
      0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, //
      0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, //
      0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, //
      -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, //
      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, //

      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f, //
      0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f, //
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //
      -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //
      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f  //
  };
  float skyboxVertices[] = {
      // positions
      -1.0f, 1.0f,  -1.0f, //
      -1.0f, -1.0f, -1.0f, //
      1.0f,  -1.0f, -1.0f, //
      1.0f,  -1.0f, -1.0f, //
      1.0f,  1.0f,  -1.0f, //
      -1.0f, 1.0f,  -1.0f, //

      -1.0f, -1.0f, 1.0f,  //
      -1.0f, -1.0f, -1.0f, //
      -1.0f, 1.0f,  -1.0f, //
      -1.0f, 1.0f,  -1.0f, //
      -1.0f, 1.0f,  1.0f,  //
      -1.0f, -1.0f, 1.0f,  //

      1.0f,  -1.0f, -1.0f, //
      1.0f,  -1.0f, 1.0f,  //
      1.0f,  1.0f,  1.0f,  //
      1.0f,  1.0f,  1.0f,  //
      1.0f,  1.0f,  -1.0f, //
      1.0f,  -1.0f, -1.0f, //

      -1.0f, -1.0f, 1.0f, //
      -1.0f, 1.0f,  1.0f, //
      1.0f,  1.0f,  1.0f, //
      1.0f,  1.0f,  1.0f, //
      1.0f,  -1.0f, 1.0f, //
      -1.0f, -1.0f, 1.0f, //

      -1.0f, 1.0f,  -1.0f, //
      1.0f,  1.0f,  -1.0f, //
      1.0f,  1.0f,  1.0f,  //
      1.0f,  1.0f,  1.0f,  //
      -1.0f, 1.0f,  1.0f,  //
      -1.0f, 1.0f,  -1.0f, //

      -1.0f, -1.0f, -1.0f, //
      -1.0f, -1.0f, 1.0f,  //
      1.0f,  -1.0f, -1.0f, //
      1.0f,  -1.0f, -1.0f, //
      -1.0f, -1.0f, 1.0f,  //
      1.0f,  -1.0f, 1.0f   //
  };

  unsigned int cubeVAO, cubeVBO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);
  glBindVertexArray(cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  unsigned int skyboxVAO, skyboxVBO;
  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  vector<std::string> faces{
      "resources/textures/skybox/right.jpg",
      "resources/textures/skybox/left.jpg",
      "resources/textures/skybox/top.jpg",
      "resources/textures/skybox/bottom.jpg",
      "resources/textures/skybox/front.jpg",
      "resources/textures/skybox/back.jpg",
  };
  unsigned int cubemapTexture = loadCubemap(faces);

  shader.use();
  shader.setInt("skybox", 0);

  skyboxShader.use();
  skyboxShader.setInt("skybox", 0);

  Model nanosuit("resources/objects/nanosuit/nanosuit.obj");

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model;
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix(
        (float)DEFAULT_WIDTH / (float)DEFAULT_HEIGHT, 0.1f, 100.0f);
    glm::vec3 cameraPosition = camera.position;

    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setVec3("cameraPos", cameraPosition);

    skyboxShader.use();
    glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
    skyboxShader.setMat4("view", skyboxView);
    skyboxShader.setMat4("projection", projection);

    // nanosuit
    shader.use();
    model = glm::mat4(1.0f);
    // translate it down so it's at the center of the scene
    model = glm::translate(model, glm::vec3(1.0f, -1.75f, 0.0f));
    // it's a bit too big for our scene, so scale it down
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    shader.setMat4("model", model);
    nanosuit.Draw(shader);
    glBindVertexArray(0);

    // cubes
    shader.use();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
    shader.setMat4("model", model);
    glBindVertexArray(cubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // draw skybox last
    glDepthFunc(GL_LEQUAL);
    skyboxShader.use();
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &skyboxVAO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &skyboxVAO);

  glfwTerminate();
  return 0;
}
