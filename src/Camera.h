#ifndef Camera_h
#define Camera_h

#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat ZOOM = 45.0f;
const GLfloat SPEED = 6.0f;
const GLfloat SENSITIVITY = 0.25f;

class Camera
{
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           GLfloat yaw = YAW,
           GLfloat pitch = PITCH)
        : front(glm::vec3(0.0f, 0.0f, -1.0f)),
          zoom(ZOOM),
          movementSpeed(SPEED),
          mouseSensitivity(SENSITIVITY)
    {
        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        this->updateCameraVectors();
    }
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(this->position, this->position + this->front, this->up);
    }
    GLfloat GetZoom()
    {
        return this->zoom;
    }
    glm::vec3 GetPosition()
    {
        return this->position;
    }
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
    {
        GLfloat velocity = this->movementSpeed * deltaTime;
        if (direction == FORWARD)
        {
            this->position += this->front * velocity;
        }
        if (direction == BACKWARD)
        {
            this->position -= this->front * velocity;
        }
        if (direction == LEFT)
        {
            this->position -= this->right * velocity;
        }
        if (direction == RIGHT)
        {
            this->position += this->right * velocity;
        }
    }
    void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset)
    {
        xOffset *= this->mouseSensitivity;
        yOffset *= this->mouseSensitivity;

        this->yaw += xOffset;
        this->pitch += yOffset;
        this->updateCameraVectors();
    }

    void ProcessMouseScroll(GLfloat amount)
    {
        zoom -= amount;
    }

private:
    GLfloat yaw;
    GLfloat pitch;
    GLfloat zoom;
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 worldUp;
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
        front.y = sin(glm::radians(this->pitch));
        front.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
        this->front = glm::normalize(front);

        this->right = glm::normalize(glm::cross(this->front, this->worldUp));

        this->up = glm::normalize(glm::cross(this->right, this->front));
    }
};

#endif