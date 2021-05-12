#pragma once

#include "stb_image.h"
#include "Shader.h"
#include "VerticesLib.h"
#include <vector>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"


class Mesh {
public:
	Mesh (const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> shader) : shader (shader), vertices (vertices), vertices_cnt (vertices_cnt), indices_cnt (indices_cnt), indices (indices) {
		glGenVertexArrays (1, &VAO);
		glGenBuffers (1, &VBO);
		glGenBuffers (1, &EBO);

		glBindVertexArray (VAO);

		glBindBuffer (GL_ARRAY_BUFFER, VBO);
		glBufferData (GL_ARRAY_BUFFER, sizeof (float) * vertices_cnt, vertices, GL_STATIC_DRAW);


		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData (GL_ELEMENT_ARRAY_BUFFER, indices_cnt * sizeof (unsigned int), indices, GL_STATIC_DRAW);
		// position attributes
		glEnableVertexAttribArray (0);
		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void*)0);

		createTexture (".\\objects\\textures\\transparent.png", false);
		// texture coord attribute
		glEnableVertexAttribArray (1);
		glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void*)(3 * sizeof (float)));

		// texture coord attribute
		glEnableVertexAttribArray (2);
		glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void*)(6 * sizeof (float)));

		glBindVertexArray (0);
	}


	int createTexture (const char* tex_path, bool flip_texture_on_load = false, bool is_png = false) {
		unsigned int texture;
		glGenTextures (1, &texture);
		glBindTexture (GL_TEXTURE_2D, texture);

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);

		int width, height, nrChannels;

		stbi_set_flip_vertically_on_load (flip_texture_on_load);
		unsigned char* data = stbi_load (tex_path, &width, &height, &nrChannels, 0);
		int mode1 = is_png ? GL_RGBA : GL_RGB;
		int mode2 = nrChannels == 3 ? GL_RGB : GL_RGBA;

		if (data) {
			glTexImage2D (GL_TEXTURE_2D, 0, mode1, width, height, 0, mode2, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap (GL_TEXTURE_2D);
		} else {
			std::cout << "failed loading textures!" << std::endl;
		}

		stbi_image_free (data);
		shader->use ();
		std::string texture_name = "texture_diffuse1";
		shader->setInt (texture_name.c_str (), texture_cnt++);
		textures.push_back (std::move (texture));
		return 1;
	}

	void Draw () const {
		glBindVertexArray (VAO);

		glActiveTexture (GL_TEXTURE0);
		glBindTexture (GL_TEXTURE_2D, textures.back ());
		shader->setInt ("texture0", 0);

		glDrawElements (GL_TRIANGLES, indices_cnt, GL_UNSIGNED_INT, 0);
		glBindVertexArray (0);
	}

	~Mesh () {
		glDeleteVertexArrays (1, &VAO);
		glDeleteBuffers (1, &VBO);
	}

private:
	const float* vertices;
	int vertices_cnt, indices_cnt;
	const unsigned int* indices;
	int texture_cnt = 0;

	unsigned int VAO, VBO, EBO;
	std::vector <unsigned int> textures;
	std::shared_ptr <Shader> shader;
};

class ObjectBase {
public:
	double getTimeSeed () {
		std::chrono::time_point<std::chrono::system_clock> now =
			std::chrono::system_clock::now ();
		auto duration = now.time_since_epoch ();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count ();
		double res = millis % (long int)10e4 / 10.0e2;
		return res;
	}

	ObjectBase (const float* vertices, const int vertices_cnt, const unsigned int* indices,
		const int indices_cnt, std::shared_ptr <Shader> shader, char color_id = 0x0)
		: shader (shader), vertices (vertices), vertices_cnt (vertices_cnt), indices_cnt (indices_cnt),
		indices (indices), color_id (color_id) {
		meshes.emplace_back (vertices, vertices_cnt, indices, indices_cnt, shader);
	}

	virtual void DrawObject () = 0;

	void Draw () {
		shader->use ();
		DrawObject ();
	}

	virtual void applyClick (int object_id) {
		std::cout << "click!" << std::endl;
	}

	void setModelMatrices (const glm::mat4& model_mat) {
		shader->setMat4 ("model", model_mat);
		glm::mat3 normal_model = glm::mat3 (glm::transpose (glm::inverse (model_mat)));
		shader->setMat3 ("normal_model", normal_model);
	}

	void setMeshMaterial (const glm::vec3& ambient, const glm::vec3& specular, float shininess) {
		shader->setVec3 ("material.ambient", ambient);
		shader->setVec3 ("material.diffuse", ambient);
		shader->setVec3 ("material.specular", specular);
		shader->setFloat ("material.shininess", shininess);
	}

	void setMeshMaterial (const glm::vec3& ambient, float shininess) {
		shader->setVec3 ("material.ambient", ambient);
		shader->setVec3 ("material.diffuse", ambient);
		shader->setVec3 ("material.specular", glm::vec3 (0.5f, 0.5f, 0.5f));
		shader->setFloat ("material.shininess", shininess);
	}

	void setMeshMaterial (float shininess) {
		shader->setVec3 ("material.ambient", glm::vec3 (1.0f, 1.0f, 1.0f));
		shader->setVec3 ("material.diffuse", glm::vec3 (1.0f, 1.0f, 1.0f));
		shader->setVec3 ("material.specular", glm::vec3 (1.0f, 1.0f, 1.0f));
		shader->setFloat ("material.shininess", shininess);
	}


	virtual ~ObjectBase () = default;

	std::shared_ptr<Shader> shader;
	std::vector <Mesh> meshes;
	char color_id;

protected:
	const float* vertices;
	int vertices_cnt, indices_cnt;
	const unsigned int* indices;
};

