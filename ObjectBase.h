#pragma once

#include "stb_image.h"
#include "Shader.h"
#include "VerticesLib.h"
#include <vector>
#include "../glm/glm/glm.hpp"
#include "../glm/glm/gtc/matrix_transform.hpp"
//#include "glm/glm/glm.hpp"
//#include "glm/glm/gtc/matrix_transform.hpp"


class Mesh {
public:
	Mesh(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> shader) : shader(shader), vertices(vertices), vertices_cnt(vertices_cnt), indices_cnt(indices_cnt), indices(indices) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices_cnt, vertices, GL_STATIC_DRAW); //puts vertices data into our vertex buffer object


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_cnt * sizeof(unsigned int), indices, GL_STATIC_DRAW);
		// position attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

		createTexture("transparent.png", false);
		// texture coord attribute
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

		// texture coord attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		glBindVertexArray(0);
	}


	int createTexture(const char* tex_path, bool flip_texture_on_load = false) {
		unsigned int texture;
		//textures.emplace_back(texture);
		//exture = textures.back();
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);

		int width, height, nrChannels;

		stbi_set_flip_vertically_on_load(flip_texture_on_load);
		unsigned char* data = stbi_load(tex_path, &width, &height, &nrChannels, 0);
		int mode = nrChannels == 3 ? GL_RGB : GL_RGBA;

		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, mode, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "failed loading textures!" << std::endl;
		}

		stbi_image_free(data);
		shader->use();
		std::string texture_name = std::string("texture") + std::to_string(texture_cnt);
		shader->setInt(texture_name.c_str(), texture_cnt++);
		textures.push_back(std::move(texture));
		return 1;
	}

	void Draw() const {
		glBindVertexArray(VAO);
		

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures.back());
		shader->setInt("texture0", 0);
				
		glDrawElements(GL_TRIANGLES, indices_cnt, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	~Mesh() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
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
	double getTimeSeed() {
		std::chrono::time_point<std::chrono::system_clock> now =
			std::chrono::system_clock::now();
		auto duration = now.time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		double res = millis % (long int)10e4 / 10.0e2;
		return res;
	}

	ObjectBase(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> shader) : shader(shader), vertices(vertices), vertices_cnt(vertices_cnt), indices_cnt(indices_cnt), indices(indices) {
		meshes.emplace_back(vertices, vertices_cnt, indices, indices_cnt, shader);
		
	}

	virtual void DrawObject() = 0; 

	void Draw() {
		shader->use();
		DrawObject();
	}


	virtual ~ObjectBase() = default;

	std::shared_ptr<Shader> shader;
	std::vector <Mesh> meshes;
	//unsigned int VBO, VAO, EBO;
	//int texture_cnt = 0;
	//std::string name;

	
	protected: 
	const float *vertices;
	int vertices_cnt, indices_cnt;
	const unsigned int * indices;
};

