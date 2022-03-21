#include "Include/Camera.h"


Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVITY),
    Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    UpdateCameraVectors();
};

Camera::Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVITY),
    Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Front + Position, Up);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, float deltaTime, bool constrainPitch = true)
{
    xoffset *= deltaTime * 180 * MouseSensitivity;
    yoffset *= deltaTime * 180 * MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (Yaw < -180.0f) {
        Yaw += 360.0f;
    }
    if (Yaw > 180.0f) {
        Yaw -= 360.0f;
    }
    if (Pitch < -89.9f) {
        Pitch = -89.9f;
    }
    if (Pitch > 89.9f) {
        Pitch = 89.9f;
    }
    

    // update Front, Right and Up Vectors using the updated Euler angles
    UpdateCameraVectors();
}

void Camera::ProcessKeyboardMovement(float forwardComponent, float rightComponent, float upComponent, float deltaTime) {

    glm::vec3 rotatedDirection;

    rotatedDirection.x = cos(glm::radians(Yaw + 90.f)) * cos(glm::radians(Pitch));
    rotatedDirection.z = sin(glm::radians(Yaw + 90.f)) * cos(glm::radians(Pitch));
    rotatedDirection = glm::normalize(rotatedDirection);

    glm::vec3 cameraDirectionVector;
    cameraDirectionVector.x = forwardComponent * cos(glm::radians(Yaw)) + rightComponent * sin(glm::radians(Yaw));
    cameraDirectionVector.z = forwardComponent * sin(glm::radians(Yaw)) - rightComponent * cos(glm::radians(Yaw));

    Position.x += MovementSpeed * cameraDirectionVector.x * deltaTime;
    Position.z += MovementSpeed * cameraDirectionVector.z * deltaTime;
    Position.y += MovementSpeed * upComponent * deltaTime;
}

void Camera::ProcessKeyboardMovement(glm::vec3 direction, float deltaTime)
{
    ProcessKeyboardMovement(direction.x, direction.y, direction.z, deltaTime);
}

void Camera::SetCameraPosition(glm::vec3 newPosition)
{
    Position = newPosition;
}

void Camera::SetCameraPosition(float xPos, float yPos, float zPos)
{
    Position.x = xPos;
    Position.y = yPos;
    Position.z = zPos;
}

void Camera::UpdateCameraVectors() {

    Front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front.y = sin(glm::radians(Pitch));
    Front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(Front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));

    return;
}


void Camera::logMessage(std::string message) {
    std::cout << message;
}
void Camera::logMessageEndl(std::string message) {
    std::cout << message << std::endl;
}