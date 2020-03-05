#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

// Defines several possible options for camera movement. Used as abstraction to
// stay away from window-system specific input methods
enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

// Default camera values
extern const float YAW;
extern const float PITCH;
extern const float SPEED;
extern const float SENSITIVITY;
extern const float ZOOM;

// An abstract camera class that processes input and calculates the
// corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
  glm::vec3 position;
  glm::vec3 front;

  // Constructor with vectors
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = YAW,
         float pitch = PITCH, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
  // Constructor with scalar values
  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
         float yaw, float pitch);

  // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
  glm::mat4 getViewMatrix();

  // Returns the projection matrix with a given aspect, near and far
  glm::mat4 getProjectionMatrix(float aspect, float near, float far);

  // Processes input received from any keyboard-like input system. Accepts input
  // parameter in the form of camera defined ENUM (to abstract it from windowing
  // systems)
  void processKeyboard(Camera_Movement direction, float deltaTime);

  // Processes input received from a mouse input system. Expects the offset
  // value in both the x and y direction.
  void processMouseMovement(float xoffset, float yoffset,
                            GLboolean constrainPitch = true);

  // Processes input received from a mouse scroll-wheel event. Only requires
  // input on the vertical wheel-axis
  void processMouseScroll(float yoffset);

private:
  // Camera Attributes
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;
  // Euler Angles
  float yaw;
  float pitch;
  // Camera options
  float movementSpeed;
  float mouseSensitivity;
  float zoom;

  // Calculates the front vector from the Camera's (updated) Euler Angles
  void updateCameraVectors();
};
