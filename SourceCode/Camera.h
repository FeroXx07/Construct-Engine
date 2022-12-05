#ifndef CAMERA_H
#define CAMERA_H

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 90.0f;

//// Custom implementation of the LookAt function
//glm::mat4 calculate_lookAt_matrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
//{
//    // 1. Position = known
//    // 2. Calculate cameraDirection
//    glm::vec3 zaxis = glm::normalize(position - target);
//    // 3. Get positive right axis vector
//    glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
//    // 4. Calculate camera up vector
//    glm::vec3 yaxis = glm::cross(zaxis, xaxis);
//
//    // Create translation and rotation matrix
//    // In glm we access elements as mat[col][row] due to column-major layout
//    glm::mat4 translation = glm::mat4(1.0f); // Identity matrix by default
//    translation[3][0] = -position.x; // Third column, first row
//    translation[3][1] = -position.y;
//    translation[3][2] = -position.z;
//    glm::mat4 rotation = glm::mat4(1.0f);
//    rotation[0][0] = xaxis.x; // First column, first row
//    rotation[1][0] = xaxis.y;
//    rotation[2][0] = xaxis.z;
//    rotation[0][1] = yaxis.x; // First column, second row
//    rotation[1][1] = yaxis.y;
//    rotation[2][1] = yaxis.z;
//    rotation[0][2] = zaxis.x; // First column, third row
//    rotation[1][2] = zaxis.y;
//    rotation[2][2] = zaxis.z;
//
//    // Return lookAt matrix as combination of translation and rotation matrix
//    return rotation * translation; // Remember to read from right to left (first translation then rotation)
//}

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::vec3 Target;
    glm::mat4 view;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    float maxZoom = ZOOM;
    Frustum frustum;
    glm::mat4x4 projection;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        UpdateCameraVectors();
        projection = glm::mat4x4(1.0);
        frustum.type = PerspectiveFrustum;
        frustum.pos = Position;
        frustum.front = Front;
        frustum.up = Up;
        frustum.verticalFov = Zoom;
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        UpdateCameraVectors();
        projection = glm::mat4x4(1.0);
        frustum.type = PerspectiveFrustum;
        frustum.pos = Position;
        frustum.front = Front;
        frustum.up = Up;
        frustum.verticalFov = Zoom;
    }

    ~Camera() {};

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        Target = Position + Front;
        view = glm::lookAt(Position, Target, Up);
        return view;
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
       LOG("Camera pos (%f, %f, %f) ", Position.x, Position.y, Position.z);
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        LOG("X offset is %f: ", xoffset);
        LOG("Y offset is %f: ", yoffset);
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        UpdateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        LOG("Zoom offset is %f: ", yoffset);
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > maxZoom)
            Zoom = maxZoom;
    }

    void LookAt(glm::vec3 pos)
    {
        Position = pos;
        UpdateCameraVectors();
    }

    // calculates the front vector from the Camera's (updated) Euler Angles
    void UpdateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
        projection = glm::mat4x4(1.0);
        frustum.type = PerspectiveFrustum;
        frustum.pos = Position;
        frustum.front = Front;
        frustum.up = Up;
        frustum.verticalFov = Zoom;
    }
};
#endif