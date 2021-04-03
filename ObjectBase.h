#pragma once

#include "stb_image.h"
#include "Shader.h"
#include "VerticesLib.h"
#include <vector>
#include "../glm/glm/glm.hpp"
#include "../glm/glm/gtc/matrix_transform.hpp"
//#include "glm/glm/glm.hpp"
//#include "glm/glm/gtc/matrix_transform.hpp"


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
		std::shared_ptr <Shader> shader, const std::string& name) : shader(shader), vertices(vertices), vertices_cnt(vertices_cnt), indices_cnt(indices_cnt), indices(indices) {
		this->name = name;
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


		// texture coord attribute
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

		// texture coord attribute
		//glEnableVertexAttribArray(2);
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		glBindVertexArray(0);
	}

	void Update() {}
	void DrawPrep() {
		shader->use();
		glBindVertexArray(VAO);
		for (size_t i = 0; i < textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
		}
	}

	
	virtual void DrawObject() = 0; 


	void Draw() {
		DrawPrep();
		DrawObject();

	}


	~ObjectBase() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	std::shared_ptr<Shader> shader;
	std::vector <unsigned int> textures;
	unsigned int VBO, VAO, EBO;
	int texture_cnt = 0;
	std::string name;

	int createTexture(const char* tex_path, unsigned int& texture, bool flip_texture_on_load = false) {
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
		textures.push_back(texture);
		return 1;
	}
	protected: 
	const float *vertices;
	int vertices_cnt, indices_cnt;
	const unsigned int * indices;
};

