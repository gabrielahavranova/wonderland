/*!
 *  @file Camera.h
 *  @author Admin
 *  @date 2021-05-12
 *  @project hopeful_semestralka
 *
 *  Declares and defines the camera.
 **/
#pragma once

#include <glad/glad.h>
#include <cmath>
#include <vector>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

enum CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT, STATIC1, STATIC2 };

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 25.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

/*!
 * Camera class for handling navigation and movement using Euler angles - yaw
 * and pitch.
 */
class Camera {
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	float Yaw;
	float Pitch;
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	/*!
	 *  Camera Constructor using default parameters
	 *
	 *      @param [in] position
	 *      @param [in] up
	 *      @param [in] yaw
	 *      @param [in] pitch
	 */
	Camera (glm::vec3 position = glm::vec3 (0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3 (0.0f, 1.0f, 0.0f), float yaw = YAW,
		float pitch = PITCH)
		: Front (glm::vec3 (0.0f, 0.0f, -1.0f)),
		MovementSpeed (SPEED),
		MouseSensitivity (SENSITIVITY),
		Zoom (ZOOM) {
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors ();
	}

	/*!
	 *  Camera Constructor using provided parameters
	 *
	 *      @param [in] posX
	 *      @param [in] posY
	 *      @param [in] posZ
	 *      @param [in] upX
	 *      @param [in] upY
	 *      @param [in] upZ
	 *      @param [in] yaw
	 *      @param [in] pitch
	 */
	Camera (float posX, float posY, float posZ, float upX, float upY, float upZ,
		float yaw, float pitch)
		: Front (glm::vec3 (0.0f, 0.0f, -1.0f)),
		MovementSpeed (SPEED),
		MouseSensitivity (SENSITIVITY),
		Zoom (ZOOM) {
		Position = glm::vec3 (posX, posY, posZ);
		WorldUp = glm::vec3 (upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors ();
	}

	/*!
	 *  Returns the camera's view matrix.
	 *
	 *      @return The view matrix.
	 */
	glm::mat4 GetViewMatrix () {
		return glm::lookAt (Position, Position + Front, Up);
	}

	/*!
	 *  Processes the key input.
	 *
	 *      @param [in] direction - wasd
	 *      @param [in] deltaTime 
	 *      @param [in] colliders 
	 */
	void processKeyInput (CameraMovement direction, float deltaTime,
		const std::vector<glm::vec3>& colliders) {
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD) {
			glm::vec3 new_pos = Position + Front * velocity;
			if (!collides (new_pos, colliders)) Position = new_pos;
		}
		if (direction == BACKWARD) {
			glm::vec3 new_pos = Position - Front * velocity;
			if (!collides (new_pos, colliders)) Position = new_pos;
		}
		if (direction == LEFT) {
			glm::vec3 new_pos = Position - Right * velocity;
			if (!collides (new_pos, colliders)) Position = new_pos;
		}
		if (direction == RIGHT) {
			glm::vec3 new_pos = Position + Right * velocity;
			if (!collides (new_pos, colliders)) Position = new_pos;
		}
		if (direction == STATIC1) {
			// std::cout << "glm::vec3(" << Position[0] << "f, " << Position[1] <<
			// "f,"
			//          << Position[2] << "f), ";
			// std::cout << "Front = glm::vec3(" << Front.x << "f, " << Front.y <<
			// "f,"
			//          << Front.z << "f);" << std::endl;
			// std::cout << "Right = glm::vec3(" << Right.x << "f, " << Right.y << "f,
			// "
			//          << Right.z << "f);" << std::endl;
			// std::cout << "Up = glm::vec3(" << Up.x << "f, " << Up.y << "f, " <<
			// Up.z
			//          << "f);" << std::endl;
			// std::cout << "Pitch = " << Pitch << "f;" << std::endl;
			// std::cout << "Yaw = " << Yaw << "f;" << std::endl;

			Position = glm::vec3 (80.6795f, 10.001f, -92.7574f),
				Front = glm::vec3 (-0.641968f, 0.137444f, 0.754312f);
			Right = glm::vec3 (-0.761539f, 0.0f, -0.648119f);
			Up = glm::vec3 (0.0890802f, 0.99051f, -0.104669f);
			Pitch = 7.89999f;
			Yaw = -229.6f;
		}

		if (direction == STATIC2) {
			Position = glm::vec3 (-88.0046f, 10.001f, 90.4846f),
				Front = glm::vec3 (0.551006f, 0.785857f, -0.280752f);
			Right = glm::vec3 (0.453992f, -0.0f, 0.891006f);
			Up = glm::vec3 (-0.700203f, 0.618409f, 0.356773f);
			Pitch = 51.8f;
			Yaw = -27.0001f;
			updateCameraVectors ();
		}

		// keeps the movement in one height
		Position.y = 10.001f;
	}

	/*!
	 *  Processes the mouse movement.
	 *
	 *      @param [in] xoffset        
	 *      @param [in] yoffset        
	 *      @param [in] constrainPitch 
	 */
	void ProcessMouseMovement (float xoffset, float yoffset,
		GLboolean constrainPitch = true) {
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch) {
			if (Pitch > 89.0f) Pitch = 89.0f;
			if (Pitch < -89.0f) Pitch = -89.0f;
		}

		updateCameraVectors ();
	}

	 /*!
	  *  Processes the mouse scroll.
	  *
	  *      @param [in] yoffset 
	  */
	void ProcessMouseScroll (float yoffset) {
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f) Zoom = 1.0f;
		if (Zoom > 45.0f) Zoom = 45.0f;
	}

private:
	/*!
	 *  Checks for potential collision at new_position.
	 *
	 *      @param [in] new_position - potential position where to move
	 *      @param [in] colliders - array of colliders
	 *
	 *      @return true if there would be collision after moving to the new_position, false otherwise
	 */
	bool collides (const glm::vec3& new_position,
		const std::vector<glm::vec3>& colliders) {
		const float plane_p[] = { 100.858f, 90.6364f, -94.5102f, -92.7731f };
		if (new_position.x > plane_p[0] || new_position.x < plane_p[2] ||
			new_position.z > plane_p[1] || new_position.z < plane_p[3])
			return true;
		for (size_t i = 0; i < colliders.size (); i++) {
			if (glm::distance (glm::vec2 (new_position.x, new_position.z),
				glm::vec2 (colliders[i].x, colliders[i].y)) <
				colliders[i].z)
				return true;
		}
		return false;
	}

	/*!
	 *  Updates the camera vectors after movement
	 */
	void updateCameraVectors () {
		glm::vec3 front;
		front.x = cos (glm::radians (Yaw)) * cos (glm::radians (Pitch));
		front.y = sin (glm::radians (Pitch));
		front.z = sin (glm::radians (Yaw)) * cos (glm::radians (Pitch));
		Front = glm::normalize (front);
		Right = glm::normalize (glm::cross (Front, WorldUp));
		Up = glm::normalize (glm::cross (Right, Front));
	}
};
