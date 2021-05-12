/*!
 *  @file Shader.h
 *  @author Gabriela Havranova
 *  @date 2021-05-12
 *  @project Wonderland
 *
 *  Shader class used for encapsulating shader programs.
 */
#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm/glm.hpp"
/*!
 *  *  Shader class used for encapsulating shader programs.
 */
class Shader {
public:
	// program ID
	unsigned int ID;

	/*!
	 *  Constructor.
	 *
	 *      @param [in] vertex_path  - path to vertex shader file
	 *      @param [in] fragment_path - path to fragment shader file
	 */
	Shader(const char* vertex_path, const char* fragment_path) {
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			vShaderFile.open(vertex_path);
			fShaderFile.open(fragment_path);
			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			vShaderFile.close();
			fShaderFile.close();
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "Error, couldn't read shader file!" << std::endl;
		}
		const char* vertex_code = vertexCode.c_str();
		const char* fragment_code = fragmentCode.c_str();
		createShaderProgram (vertex_code, fragment_code);
	}

	bool createShaderProgram (const char* vertex_code, const char* fragment_code) {
		unsigned int vertex, fragment;
		int success;
		char info_log[512];

		// vertex shader
		vertex = glCreateShader (GL_VERTEX_SHADER);
		glShaderSource (vertex, 1, &vertex_code, NULL);
		glCompileShader (vertex);

		glGetShaderiv (vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog (vertex, 512, NULL, info_log);
			std::cout << "Error: couldn't compile vertex shader!\n" << info_log << std::endl;
			return false;
		}

		fragment = glCreateShader (GL_FRAGMENT_SHADER);
		glShaderSource (fragment, 1, &fragment_code, NULL);
		glCompileShader (fragment);

		glGetShaderiv (fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog (fragment, 512, NULL, info_log);
			std::cout << "Error: couldn't compile fragment shader!\n" << info_log << std::endl;
			return false;
		}

		// shader program 
		ID = glCreateProgram ();
		glAttachShader (ID, vertex);
		glAttachShader (ID, fragment);
		glLinkProgram (ID);

		glGetProgramiv (ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog (ID, 512, NULL, info_log);
			std::cout << "Error: couldn't link shaders!\n" << info_log << std::endl;
			return false;
		}

		glDeleteShader (vertex);
		glDeleteShader (fragment);
		return true;
	}

	~Shader() {
		glDeleteProgram(ID);
	}

	// use/activate the shader
	void use() { glUseProgram(ID); }

	/* shader setter methods to simplify setting uniforms */
	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setMat3(const std::string& name, const glm::mat3& mat) const {
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void setMat4(const std::string& name, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void setVec3(const std::string& name, const glm::vec3& vec) const {
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0] );
	}
};