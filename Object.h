#pragma once
#include "stb_image.h"
#include "Shader.h"
#include "VerticesLib.h"
#include <vector>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"


class Object {
public:
	double getTimeSeed() {
		std::chrono::time_point<std::chrono::system_clock> now =
			std::chrono::system_clock::now();
		auto duration = now.time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		double res = (millis % 10000) / 1000.0;
		return res;
	}

	Object(const float * vertices, std::shared_ptr <Shader> shader): shader(shader) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)*180, vertices, GL_STATIC_DRAW); //puts vertices data into our vertex buffer object

		// position attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	void Update() {}
	void DrawPrep() {
		for (size_t i = 0; i < textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
		}
		glBindVertexArray(VAO);
		shader->use();
	}

	void DrawBoxes() {
		for (unsigned int i = 0; i < 10; i++) {

			glm::mat4 model1 = glm::mat4(1.0f);
			model1 = glm::translate(model1, cubePositions[i]);
			float angle = 20.0f * i;
			if (i < 5) {
				model1 = glm::rotate(model1, (float)getTimeSeed() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			else model1 = glm::rotate(model1, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			shader->setMat4("model", model1);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

	void Draw() {
		DrawBoxes();
	}


	~Object() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	std::shared_ptr<Shader> shader;
	std::vector <unsigned int> textures;
	unsigned int VBO, VAO;
	int texture_cnt = 0; 

	int createTexture(const char* tex_path, unsigned int & texture, bool flip_texture_on_load = false) {
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
};

