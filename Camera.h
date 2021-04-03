#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include "../glm/glm/glm.hpp"
//#include "glm/glm/glm.hpp"
#include "../glm/glm/gtc/matrix_transform.hpp"
#include <cmath>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    STATIC1,
    STATIC2
};


// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


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
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
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
        if (direction == STATIC1) {
           // glm::vec3 front;
            //front.x = cos(glm::radians(20.0f)) * cos(glm::radians(20.0f));
            //front.y = sin(glm::radians(20.0f));
          //  front.z = sin(glm::radians(20.0f)) * cos(glm::radians(20.0f));
            //Front = glm::normalize(front);
            //Position = glm::vec3(-24.3f, -0.22f, 18.0f);
            //Front = glm::vec3(0.696716f, 0.0697564f, -0.713947f);
            //Pitch = std::asin(0.0697564f);


            std::cout << "Position = glm::vec3(" << Position[0] << "f, " << Position[1] << "f, " << Position[2] << "f);" << std::endl;
            std::cout << "Front = glm::vec3(" << Front.x << "f, " << Front.y << "f, " << Front.z << "f);" << std::endl;
            std::cout << "Right = glm::vec3(" << Right.x << "f, " << Right.y << "f, " << Right.z << "f);" << std::endl;
            std::cout << "Up = glm::vec3(" << Up.x << "f, " << Up.y << "f, " << Up.z << "f);" << std::endl;
            std::cout << "Pitch = " << Pitch << "f;" << std::endl;
            std::cout << "Yaw = " << Yaw << "f;" << std::endl;

            Pitch = 0.400004f;
            Yaw = 318.3f;
            Position = glm::vec3(-25.051f, -0.572113f, 18.8281f);
            Zoom = 45.0f;
            updateCameraVectors();
            //Front = glm::vec3(0.620842f, -0.0314108f, -0.783306f);
           // Right = glm::vec3(0.209618f, 0.0f, -0.977783f);
            //Up = glm::vec3(-0.430165f, 0.898028f, -0.0922193f);
            
        }
        if (direction == STATIC2) {
            Pitch = -25.3f;
            Yaw = 216.1f;
            Position = glm::vec3(66.6536f, 37.5282f, 30.5252f);
            updateCameraVectors();
        }
           // Front = glm::vec3(-0.317231f, - 0.020943f, - 0.948117f);
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
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
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
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
    }
};
#endif

