#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <string>

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.75f;
const float ZOOM = 45.0f;

class Camera {


public:

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    glm::mat4 GetViewMatrix();

    void ProcessMouseMovement(float xoffset, float yoffset, float deltaTime, bool constrainPitch);

    void ProcessKeyboardMovement(float forwardComponent, float rightComponent, float upComponent, float deltaTime);

    void ProcessKeyboardMovement(glm::vec3 direction, float deltaTime);

    void SetCameraPosition(glm::vec3 newPosition);

    void SetCameraPosition(float xPos, float yPos, float zPos);

    void logMessage(std::string message);
    void logMessageEndl(std::string message);

private:

    void UpdateCameraVectors();

};