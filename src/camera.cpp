#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

Camera::Camera(glm::vec3 position, float yaw, float pitch, glm::vec3 up)
    : position(position), front(glm::vec3(0.0f, 0.0f, -1.0f)), worldUp(up),
      yaw(yaw), pitch(pitch), movementSpeed(SPEED),
      mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
  updateCameraVectors();
}
Camera::Camera(float posX, float posY, float posZ, float upX, float upY,
               float upZ, float yaw, float pitch) {
  Camera(glm::vec3(posX, posY, posZ), yaw, pitch, glm::vec3(upX, upY, upZ));
}

glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspect, float near, float far) {
  return glm::perspective(glm::radians(zoom), aspect, near, far);
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime) {
  float velocity = movementSpeed * deltaTime;
  if (direction == FORWARD) {
    position += front * velocity;
  }
  if (direction == BACKWARD) {
    position -= front * velocity;
  }
  if (direction == LEFT) {
    position -= right * velocity;
  }
  if (direction == RIGHT) {
    position += right * velocity;
  }
  if (direction == UP) {
    position += up * velocity;
  }
  if (direction == DOWN) {
    position -= up * velocity;
  }
}

void Camera::processMouseMovement(float xoffset, float yoffset,
                                  GLboolean constrainPitch) {
  xoffset *= mouseSensitivity;
  yoffset *= mouseSensitivity;

  yaw += xoffset;
  pitch += yoffset;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch) {
    if (pitch > 89.0f) {
      pitch = 89.0f;
    }
    if (pitch < -89.0f) {
      pitch = -89.0f;
    }
  }

  updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset) {
  if (zoom >= 1.0f && zoom <= 45.0f) {
    zoom -= yoffset;
  }
  if (zoom <= 1.0f) {
    zoom = 1.0f;
  }
  if (zoom >= 45.0f) {
    zoom = 45.0f;
  }
}

void Camera::updateCameraVectors() {
  glm::vec3 newfront;
  newfront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  newfront.y = sin(glm::radians(pitch));
  newfront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(newfront);
  right = glm::normalize(glm::cross(
      front, worldUp)); // Normalize the vectors, because their length gets
                        // closer to 0 the more you look up or down which
                        // results in slower movement.
  up = glm::normalize(glm::cross(right, front));
}
