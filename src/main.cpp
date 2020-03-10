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
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);

  Shader basicShader("./shaders/basic.vert", "./shaders/basic.frag");
  Shader transparentShader("./shaders/basic.vert",
                           "./shaders/transparent.frag");
  Shader lightingShader("./shaders/colors.vert", "./shaders/colors.frag");
  Shader lampShader("./shaders/lamp.vert", "./shaders/lamp.frag");
  Shader shaderSingleColor("shaders/stencil-test.vert",
                           "shaders/stencil-test.frag");

  float vertices[] = {
      // positions         // normals           // texture coords
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f, //
      0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, //
      0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f, //
      0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, //
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f, //
      -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f, //

      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //
      0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, //
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //
      -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, //
      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //

      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, //
      -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 1.0f, //
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, //
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, //
      -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 0.0f, //
      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, //

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, //
      0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f, //
      0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f, //
      0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f, //
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, //
      0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, //

      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f, //
      0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f, //
      0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f, //
      0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f, //
      -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 0.0f, //
      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f, //

      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, //
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, //
      0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f, //
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, //
      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, //
      -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f  //
  };

  float planeVertices[] = {
      // positions         // texture coords
      5.0f,  -0.5f, 5.0f,  2.0f, 0.0f, //
      -5.0f, -0.5f, -5.0f, 0.0f, 2.0f, //
      -5.0f, -0.5f, 5.0f,  0.0f, 0.0f, //

      5.0f,  -0.5f, 5.0f,  2.0f, 0.0f, //
      5.0f,  -0.5f, -5.0f, 2.0f, 2.0f, //
      -5.0f, -0.5f, -5.0f, 0.0f, 2.0f  //
  };

  float transparentVertices[] = {
      // positions       // texture Coords
      // (swapped y coordinates because the texture is flipped upside down)
      0.0f, 0.5f,  0.0f, 0.0f, 0.0f, //
      0.0f, -0.5f, 0.0f, 0.0f, 1.0f, //
      1.0f, -0.5f, 0.0f, 1.0f, 1.0f, //

      0.0f, 0.5f,  0.0f, 0.0f, 0.0f, //
      1.0f, -0.5f, 0.0f, 1.0f, 1.0f, //
      1.0f, 0.5f,  0.0f, 1.0f, 0.0f  //
  };

  glm::vec3 cubePositions[] = {
      glm::vec3(-3.0f, 0.0f, 0.0f),   glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  glm::vec3 pointLightPositions[] = {
      glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f),
      glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};

  unsigned int cubeVBO;
  glGenBuffers(1, &cubeVBO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  unsigned int cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glBindVertexArray(cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  unsigned int lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  unsigned int planeVAO, planeVBO;
  glGenVertexArrays(1, &planeVAO);
  glGenBuffers(1, &planeVBO);
  glBindVertexArray(planeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glBindVertexArray(0);

  unsigned int transparentVAO, transparentVBO;
  glGenVertexArrays(1, &transparentVAO);
  glGenBuffers(1, &transparentVBO);
  glBindVertexArray(transparentVAO);
  glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices),
               transparentVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glBindVertexArray(0);

  unsigned int diffuseMap = loadTexture("./resources/textures/container2.png");
  unsigned int specularMap =
      loadTexture("./resources/textures/container2_specular.png");
  unsigned int floorTexture = loadTexture("resources/textures/metal.png");
  unsigned int grassTexture = loadTexture("resources/textures/grass.png");
  unsigned int windowTexture = loadTexture("resources/textures/window.png");

  vector<glm::vec3> vegetation{
      glm::vec3(-1.5f, 0.0f, -0.48f), glm::vec3(1.5f, 0.0f, 0.51f),
      glm::vec3(0.0f, 0.0f, 0.7f), glm::vec3(-0.3f, 0.0f, -2.3f),
      glm::vec3(0.5f, 0.0f, -0.6f)};

  vector<glm::vec3> windows{
      glm::vec3(-1.5f, 0.0f, -0.48f), glm::vec3(1.5f, 0.0f, 0.51f),
      glm::vec3(0.0f, 0.0f, 0.7f), glm::vec3(-0.3f, 0.0f, -2.3f),
      glm::vec3(0.5f, 0.0f, -0.6f)};

  basicShader.use();
  basicShader.setInt("texture1", 0);
  transparentShader.use();
  transparentShader.setInt("texture1", 0);
  lightingShader.use();
  lightingShader.setInt("material.diffuse", 0);
  lightingShader.setInt("material.specular", 1);

  Model nanosuit("resources/objects/nanosuit/nanosuit.obj");

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    // sort the transparent windows before rendering
    std::map<float, glm::vec3> sortedWindows;
    for (unsigned int i = 0; i < windows.size(); i++) {
      float distance = glm::length(camera.position - windows[i]);
      sortedWindows[distance] = windows[i];
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix(
        (float)DEFAULT_WIDTH / (float)DEFAULT_HEIGHT, 0.1f, 100.0f);
    lightingShader.use();
    lightingShader.setMat4("projection", projection);
    lightingShader.setMat4("view", view);
    lampShader.use();
    lampShader.setMat4("projection", projection);
    lampShader.setMat4("view", view);
    basicShader.use();
    basicShader.setMat4("projection", projection);
    basicShader.setMat4("view", view);
    transparentShader.use();
    transparentShader.setMat4("projection", projection);
    transparentShader.setMat4("view", view);
    shaderSingleColor.use();
    shaderSingleColor.setMat4("projection", projection);
    shaderSingleColor.setMat4("view", view);

    // lighting
    lightingShader.use();
    lightingShader.setVec3("viewPos", camera.position);
    lightingShader.setFloat("material.shininess", 64.0f);

    // directional light
    lightingShader.setVec3("directionLights[0].base.ambient", 0.05f, 0.05f,
                           0.05f);
    lightingShader.setVec3("directionLights[0].base.diffuse", 0.4f, 0.4f, 0.4f);
    lightingShader.setVec3("directionLights[0].base.specular", 0.5f, 0.5f,
                           0.5f);
    lightingShader.setVec3("directionLights[0].direction", -0.2f, -1.0f, -0.3f);
    // point light 1
    lightingShader.setVec3("pointLights[0].base.ambient", 0.05f, 0.05f, 0.05f);
    lightingShader.setVec3("pointLights[0].base.diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader.setVec3("pointLights[0].base.specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
    lightingShader.setFloat("pointLights[0].constant", 1.0f);
    lightingShader.setFloat("pointLights[0].linear", 0.09);
    lightingShader.setFloat("pointLights[0].quadratic", 0.032);
    // point light 2
    lightingShader.setVec3("pointLights[1].base.ambient", 0.05f, 0.05f, 0.05f);
    lightingShader.setVec3("pointLights[1].base.diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader.setVec3("pointLights[1].base.specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
    lightingShader.setFloat("pointLights[1].constant", 1.0f);
    lightingShader.setFloat("pointLights[1].linear", 0.09);
    lightingShader.setFloat("pointLights[1].quadratic", 0.032);
    // point light 3
    lightingShader.setVec3("pointLights[2].base.ambient", 0.05f, 0.05f, 0.05f);
    lightingShader.setVec3("pointLights[2].base.diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader.setVec3("pointLights[2].base.specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
    lightingShader.setFloat("pointLights[2].constant", 1.0f);
    lightingShader.setFloat("pointLights[2].linear", 0.09);
    lightingShader.setFloat("pointLights[2].quadratic", 0.032);
    // point light 4
    lightingShader.setVec3("pointLights[3].base.ambient", 0.05f, 0.05f, 0.05f);
    lightingShader.setVec3("pointLights[3].base.diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader.setVec3("pointLights[3].base.specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
    lightingShader.setFloat("pointLights[3].constant", 1.0f);
    lightingShader.setFloat("pointLights[3].linear", 0.09);
    lightingShader.setFloat("pointLights[3].quadratic", 0.032);
    // spotLight
    lightingShader.setVec3("spotLights[0].point.base.ambient", 0.0f, 0.0f,
                           0.0f);
    lightingShader.setVec3("spotLights[0].point.base.diffuse", 1.0f, 1.0f,
                           1.0f);
    lightingShader.setVec3("spotLights[0].point.base.specular", 1.0f, 1.0f,
                           1.0f);
    lightingShader.setVec3("spotLights[0].point.position", camera.position);
    lightingShader.setFloat("spotLights[0].point.constant", 1.0f);
    lightingShader.setFloat("spotLights[0].point.linear", 0.09);
    lightingShader.setFloat("spotLights[0].point.quadratic", 0.032);
    lightingShader.setVec3("spotLights[0].direction", camera.front);
    lightingShader.setFloat("spotLights[0].cutOff",
                            glm::cos(glm::radians(12.5f)));
    lightingShader.setFloat("spotLights[0].outerCutOff",
                            glm::cos(glm::radians(15.0f)));

    // nanosuit
    lightingShader.use();
    glm::mat4 model = glm::mat4(1.0f);
    // translate it down so it's at the center of the scene
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
    // it's a bit too big for our scene, so scale it down
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    lightingShader.setMat4("model", model);
    nanosuit.Draw(lightingShader);

    // lamps
    lampShader.use();
    glBindVertexArray(lightVAO);
    for (unsigned int i = 0; i < 4; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, pointLightPositions[i]);
      model = glm::scale(model, glm::vec3(0.2f));
      lampShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // floor
    basicShader.use();
    glBindVertexArray(planeVAO);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
    basicShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // cubes
    lightingShader.use();
    glBindVertexArray(cubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    for (unsigned int i = 0; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      lightingShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // vegetation
    transparentShader.use();
    glBindVertexArray(transparentVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grassTexture);
    for (GLuint i = 0; i < vegetation.size(); i++) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, vegetation[i]);
      model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
      transparentShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    basicShader.use();
    glBindVertexArray(transparentVAO);
    glBindTexture(GL_TEXTURE_2D, windowTexture);
    for (std::map<float, glm::vec3>::reverse_iterator it =
             sortedWindows.rbegin();
         it != sortedWindows.rend(); ++it) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, it->second);
      basicShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &lightVAO);
  glDeleteVertexArrays(1, &planeVAO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &planeVBO);

  glfwTerminate();
  return 0;
}
